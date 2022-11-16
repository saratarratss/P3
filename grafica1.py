import matplotlib.pyplot as plt
import numpy as np
import soundfile as sf

signal, fm=sf.read('prueba.wav')
time=np.arange(0,600)/fm
t1=15000
t2=t1+600
plt.subplot(2,1,1)
plt.title('Pitch analysis')
plt.plot(time,signal[t1:t2])
plt.xlabel('Time(s)')
plt.ylabel('Amplitude')
plt.grid(True)

plt.subplot(2,1,2)
plt.xlabel('Samples')
plt.ylabel('Autocorrelation')
plt.acorr(signal[t1:t2], maxlags=300)
plt.grid(True)

plt.show() 