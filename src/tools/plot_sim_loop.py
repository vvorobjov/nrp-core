import numpy as np
import matplotlib.pyplot as plt
import sys

line_styles = ['-','--','-.',':']


def plot_loop(folder_name, s):
    print(f'\nplotting folder "{folder_name}" with line style "{s}"')
    begin = np.genfromtxt(folder_name+"/step_start.log", delimiter=' ', skip_header=0)
    wait = np.genfromtxt(folder_name+"/after_wait_for_engines.log", delimiter=' ', skip_header=0)
    get = np.genfromtxt(folder_name+"/after_get_datapacks.log", delimiter=' ', skip_header=0)
    run = np.genfromtxt(folder_name+"/after_run_tfs.log", delimiter=' ', skip_header=0)
    send = np.genfromtxt(folder_name+"/after_send_datapacks.log", delimiter=' ', skip_header=0)
    start = np.genfromtxt(folder_name+"/after_restart_engines.log", delimiter=' ', skip_header=0)
    sim_loop = np.genfromtxt(folder_name+"/step_duration.log", delimiter=' ', skip_header=0)
    plt.plot((wait-begin) * 1e-3, color='tab:blue', linestyle=s, label='wait for engines step completion')
    plt.plot((get-wait) * 1e-3, color='tab:orange', linestyle=s, label='get datapacks')
    plt.plot((run-get) * 1e-3, color='tab:green', linestyle=s, label='run tfs')
    plt.plot((send-run) * 1e-3, color='tab:red', linestyle=s, label='send datapacks')
    plt.plot((start-send) * 1e-3, color='tab:purple', linestyle=s, label='restart engines')
    plt.plot(sim_loop * 1e-3, color='tab:gray', linestyle=s, label='loop step duration')

    with open(folder_name+"/experiment_stats.log") as f:
        stats = f.readlines()

    print('')
    stats = stats[-1:]+stats[:-1]
    for s in stats:
        l = s.split(' ')
        t = ' '.join(l[:-1])
        n = int(l[-1]) * 1e-3
        print(f'{t} {n} (ms)')
    print('')

    print(f'loop step duration (mean): {sim_loop.mean() * 1e-3} (ms)')
    print(f'wait for engines step completion (mean): {(wait-begin).mean() * 1e-3} (ms)')
    print(f'get datapacks (mean): {(get-wait).mean() * 1e-3} (ms)')
    print(f'run tfs (mean): {(run-get).mean() * 1e-3} (ms)')
    print(f'send datapacks (mean): {(send-run).mean() * 1e-3} (ms)')
    print(f'restart engines (mean): {(start-send).mean() * 1e-3} (ms)')


# plot data from each folder name passed as arguments up to the maximum number of line styles available
if len(sys.argv) - 1 > len(line_styles):
    print(f"Warning: some of the log folders won't be plotted. The maximum number allowed is {len(line_styles)}")

n = min(len(sys.argv), len(line_styles) + 1)
for i in range(1, n):
    folder_name = sys.argv[i]
    line_style = line_styles[i-1]
    plot_loop(folder_name, line_style)

plt.legend()
plt.xlabel('loop step')
plt.ylabel('ms')
plt.show()
