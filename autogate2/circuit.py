from .autogate_plugin import Circuit

class CircuitDummy(object):

    # => ASCII Circuit Diagrams <= #

    def __str__(
        self,
        ):

        """ String representation of this Circuit (an ASCII circuit diagram). """
        # return self.ascii_diagram(time_lines='both')
        return self.ascii_diagram2()

    ascii_diagram_max_width = 80
        
    def ascii_diagram2(
        self,
        max_width=None,
        ):

        max_width = Circuit.ascii_diagram_max_width if max_width is None else max_width

        # => Utility Class <= #

        class GatePrintingLayout(object):
            
            def __init__(
                self,
                min_qubit=0,
                max_qubit=-1,
                ascii_symbols=[], # None to indicate vertical connector
                ):
            
                self.min_qubit = min_qubit
                self.max_qubit = max_qubit
                self.ascii_symbols = ascii_symbols
                
            @property
            def ascii_symbol_enumeration(self):
                return [(qubit, symbol) for qubit, symbol in zip(range(self.min_qubit, self.max_qubit+1), self.ascii_symbols)]
                
            @property
            def max_ascii_width(self):
                return max(len(_) for _ in self.ascii_symbols if _ is not None)
            
            @staticmethod
            def interferes(layout1, layout2):
                if layout1.min_qubit > layout2.max_qubit: return False
                if layout1.max_qubit < layout2.min_qubit: return False
                return True
                
            @staticmethod
            def build(qubits, ascii_symbols):
                min_qubit = min(qubits)
                max_qubit = max(qubits)
                nqubit = max_qubit - min_qubit + 1
                ascii_symbols2 = [None] * nqubit
                for qubit, ascii_symbol in zip(qubits, ascii_symbols):
                    ascii_symbols2[qubit - min_qubit] = ascii_symbol
                return GatePrintingLayout(
                    min_qubit=min_qubit,
                    max_qubit=max_qubit,
                    ascii_symbols=ascii_symbols2,
                    )

        # => Logical Layout <= #
            
        # Map of time : list of [list of GatePrintingLayout]
        # Portion in [] represents a relative second within the time index
        layouts = { time : [[]] for time in range(0, self.max_time+1) }
        # List of (starting second, ending second) for all multi-time gates
        time_connector_keys = []
        # GatePrintingLayouts for all multi-time gates
        time_connector_layouts = []
        for key, gate in self.gates.items():
            time, qubits = key
            layout = GatePrintingLayout.build(qubits, gate.ascii_symbols)
            time_connector_key = []
            found = False
            for second, layouts2 in enumerate(layouts[time]):
                if any(GatePrintingLayout.interferes(layout, _) for _ in layouts2): 
                    continue
                layouts2.append(layout)
                found = True
                break
            if not found:
                second += 1
                layouts[time].append([layout])
            time_connector_key.append((time, second))
                    
        # List of tuple of (time, relative_second) for all absolute seconds
        seconds = []
        for time, second_layouts in layouts.items():
            seconds += [(time, _) for _ in range(len(second_layouts))]
            
        # Reverse map of (time, relative_second) : absolute second
        seconds_map = { v : k for k, v in enumerate(seconds) }
        
        # => ASCII Sizes <= #
        
        # Determine ASCII widths of each second
        seconds_ascii_widths = [max([_.max_ascii_width for _ in layouts[time][second]] + [0]) for time, second in seconds]
        
        # Adjust ASCII widths for time width
        time_widths_header = { time : len(str(time)) for time in range(0, self.max_time+1)}
        time_widths = { time : 0 for time in range(0, self.max_time+1)}
        for second_index, key in enumerate(seconds):
            time, second = key
            time_widths[time] += seconds_ascii_widths[second_index]
        for time, second_layouts in layouts.items():
            if len(second_layouts) > 0: time_widths[time] += len(second_layouts) - 1
        time_adjustments = { time : max(time_widths_header[time] - time_widths[time], 0) 
            for time in time_widths.keys()}
        for time, second_layouts in layouts.items():
            if len(second_layouts) == 0: continue
            second = len(second_layouts)-1 # Last second in moment gets adjusted
            second_index = seconds_map[(time, second)]
            seconds_ascii_widths[second_index] += time_adjustments[time]
        
        # Adjust ASCII widths for separation characters
        seconds_ascii_widths = [_ + 1 for _ in seconds_ascii_widths]
        
        # Determine ASCII starts of each second
        seconds_ascii_starts = [0]
        for index, width in enumerate(seconds_ascii_widths):
            seconds_ascii_starts.append(seconds_ascii_starts[index] + width)
        ascii_width = seconds_ascii_starts[-1] # Total width
        seconds_ascii_starts = seconds_ascii_starts[:-1]
        
        # => ASCII Art <= #
        
        nqubit = self.nqubit
        min_qubit = 0
        
        wire_lines = [['-'] * ascii_width for _ in range(nqubit)]
        join_lines = [[' '] * ascii_width for _ in range(nqubit)]
        
        for time_connector_key, time_connector_layout in zip(time_connector_keys, time_connector_layouts):
            second1 = time_connector_key[0]
            second2 = time_connector_key[1]
            second1_index = seconds_map[second1]
            second2_index = seconds_map[second2]
            second1_start = seconds_ascii_starts[second1_index]
            second2_start = seconds_ascii_starts[second2_index]
            for qubit, symbol in time_connector_layout.ascii_symbol_enumeration[:-1]:
                join_lines[qubit - min_qubit][second1_start:second2_start] = '*' * (second2_start - second1_start)
            for qubit, symbol in time_connector_layout.ascii_symbol_enumeration:
                if symbol is None: continue
                wire_lines[qubit - min_qubit][second1_start:second2_start] = '=' * (second2_start - second1_start)
        
        for time, second_layouts in layouts.items():
            for second, layouts2 in enumerate(second_layouts):
                second_index = seconds_map[(time, second)]
                second_start = seconds_ascii_starts[second_index]
                for layout in layouts2:
                    for qubit in range(layout.min_qubit, layout.max_qubit+1):
                        wire_lines[qubit - min_qubit][second_start] = '|'
                    for qubit in range(layout.min_qubit, layout.max_qubit):
                        join_lines[qubit - min_qubit][second_start] = '|'
                    for qubit, symbol in layout.ascii_symbol_enumeration:
                        if symbol is None: continue
                        for index, char in enumerate(symbol):
                            wire_lines[qubit - min_qubit][second_start + index] = char      
        
        # => Assembly <= #

        wire_strs = [''.join(_) for _ in wire_lines]
        join_strs = [''.join(_) for _ in join_lines]
        time_str = ''.join(['%-*d|' % (v, k) for k, v in time_widths.items()])
        
        qwidth = max(len(str(index + min_qubit)) for index in range(nqubit)) if nqubit else 0
        
        # Pagination
        fwidth = qwidth + 6 # Width of first column
        effective_width = max_width - (qwidth + 6)
        page_starts = [0]
        for time in range(0, self.max_time+1):
            second_index = seconds_map[(time, 0)]
            second_ascii_start = seconds_ascii_starts[second_index]
            if second_ascii_start - page_starts[-1] > effective_width:
                second_index2 = seconds_map[(time - 1, 0)]
                second_ascii_start2 = seconds_ascii_starts[second_index2]
                if second_ascii_start2 == page_starts[-1]:
                    raise RuntimeError('time index %d is too large to fit' % time)
                page_starts.append(second_ascii_start2)
        page_starts.append(ascii_width)

        ascii_str = ''
        for page_index in range(len(page_starts) - 1):
            page_start = page_starts[page_index]
            page_stop = page_starts[page_index+1]

            ascii_str += 'T%*s : %s%s\n' % (
                qwidth,
                ' ',
                '|' if page_index == 0 else ' ',
                time_str[page_start:page_stop],
                )

            ascii_str += '\n'
    
            for index, wire_str in enumerate(wire_strs): 
                join_str = join_strs[index]
                ascii_str += 'q%-*d : %s%s\n' % (
                    qwidth,
                    index + min_qubit,
                    '-' if page_index == 0 else ' ',
                    wire_str[page_start:page_stop],
                    )
                ascii_str += ' %-*s   %s%s\n' % (
                    qwidth,
                    ' ',
                    ' ',
                    join_str[page_start:page_stop],
                    )

            ascii_str += 'T%*s : %s%s\n' % (
                qwidth,
                ' ',
                '|' if page_index == 0 else ' ',
                time_str[page_start:page_stop],
                )

            if page_index < len(page_starts) - 2:
                ascii_str += '\n'
        
        return ascii_str

Circuit.__str__ = CircuitDummy.__str__
Circuit.ascii_diagram_max_width = CircuitDummy.ascii_diagram_max_width
Circuit.ascii_diagram2 = CircuitDummy.ascii_diagram2

