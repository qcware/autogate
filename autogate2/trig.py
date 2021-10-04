from .autogate_plugin import TrigMonomial

def _trig_monomial_str(self):
    variables = self.variables
    if len(variables) == 0:
        return '1'
    return 'exp(1j*(%s))' % ('+'.join(['%d%s' % (order, symbol) for symbol, order in variables]))
    
TrigMonomial.__str__ = _trig_monomial_str

from .autogate_plugin import TrigPolynomial

def _trig_polynomial_str(self):
    polynomial = self.polynomial
    if len(polynomial) == 0:
        return '0'
    s = ''
    for monomial, coefficient in polynomial.items():
        s += '+(%24.16E + %24.16Ej) * %s\n' % (coefficient.real, coefficient.imag, monomial)
    return s
    
TrigPolynomial.__str__ = _trig_polynomial_str
