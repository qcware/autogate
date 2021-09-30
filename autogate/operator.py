class Operator(object):

    @staticmethod
    def build_operator(
        *,
        circuit,
        nqubit=None,
        ):

        nqubit = circuit.nqubit if nqubit is None else nqubit
        
        
