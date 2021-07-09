import numpy as np
import matplotlib.pyplot as plt
import sys

def plot_loop(dir, s):
    begin = np.genfromtxt(dir+"/begin.log", delimiter=' ', skip_header=0)
    wait = np.genfromtxt(dir+"/wait.log", delimiter=' ', skip_header=0)
    get = np.genfromtxt(dir+"/get.log", delimiter=' ', skip_header=0)
    run = np.genfromtxt(dir+"/run.log", delimiter=' ', skip_header=0)
    send = np.genfromtxt(dir+"/send.log", delimiter=' ', skip_header=0)
    start = np.genfromtxt(dir+"/start.log", delimiter=' ', skip_header=0)
    sim_loop = np.genfromtxt(dir+"/sim_loop.log", delimiter=' ', skip_header=0)
    plt.plot((wait-begin) * 1e-3, color='tab:blue', linestyle=s, label='wait')
    plt.plot((get-wait) * 1e-3, color='tab:orange', linestyle=s, label='get')
    plt.plot((run-get) * 1e-3, color='tab:green', linestyle=s, label='run')
    plt.plot((send-run) * 1e-3, color='tab:red', linestyle=s, label='send')
    plt.plot((start-send) * 1e-3, color='tab:purple', linestyle=s, label='start')
    plt.plot(sim_loop * 1e-3, color='tab:gray', linestyle=s, label='loop')
    print('{} {} {} {} {} {}'.format(sim_loop.mean() * 1e-3, (wait-begin).mean() * 1e-3,(get-wait).mean() * 1e-3,(run-get).mean() * 1e-3,(send-run).mean() * 1e-3,(start-send).mean() * 1e-3))

styles = ['-','--','-.',':']

n = min(len(sys.argv), len(styles) + 1)

for i in range(1, n):
    plot_loop(sys.argv[i], styles[i-1])

plt.legend()
plt.show()
