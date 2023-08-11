import numpy as np

def fft(signal):
    n = len(signal)

    if n <= 1:
        return signal

    even = fft(signal[0::2])
    odd = fft(signal[1::2])

    twiddle_factor = np.exp(-2j * np.pi * np.arange(n) / n)
    transformed = np.concatenate([even + twiddle_factor[:n//2] * odd,
                                  even + twiddle_factor[n//2:] * odd])

    return transformed