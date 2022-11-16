import numpy as np
import matplotlib.pyplot as plt

f0 = np.loadtxt("prueba.f0", dtype=float)
f0ref = np.loadtxt("prueba.f0ref", dtype=float)

fm = 200/3
time = np.arange(0,len(f0)).astype(float)/fm

plt.subplot(121)
plt.title("Calculated Pitch")
plt.plot(time, f0)
plt.axis([0,3,0,500])
plt.grid(True)
plt.xlabel('Time (s)')
plt.ylabel('Hz')

plt.subplot(122)
plt.title("Reference Pitch")
plt.xlabel('Time(s)')
plt.ylabel('Hz')
plt.grid(True)
plt.axis([0,3,0,500])
plt.plot(time, f0ref)

plt.show()