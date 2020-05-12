import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
import pandas as pd
from celluloid import Camera

def plot_regression(start_ind, end_ind, frame):
    m, b = np.polyfit(df_x[start_ind:end_ind, frame], df_y[start_ind:end_ind, frame], 1)
    x = np.linspace(0, 1280, 100)
    y = m*x+b
    plt.plot(x, y, color='red')
    return m # we only need the slope of each line to calculate angle

h5_path = r'C:\Users\Ma990\OneDrive - Tufts\Jobs\SPEL\Whisking\Whisking_with_Midline-Frank Ma-2020-05-03\videos\iteration-1\6400ratDLC_resnet50_Whisking_with_MidlineMay3shuffle1_500000filtered.h5'
data = pd.read_hdf(h5_path)
DLCscorer = 'DLC_resnet50_Whisking_with_MidlineMay3shuffle1_500000'
bodyparts2plot = list(data.columns.levels[1]) # https://stackoverflow.com/questions/34082137/how-to-get-pandas-column-multiindex-names-as-a-list
nframes = 5305
df_likelihood = np.empty((len(bodyparts2plot),nframes))
df_x = np.empty((len(bodyparts2plot),nframes))
df_y = np.empty((len(bodyparts2plot),nframes))

for bpindex, bp in enumerate(bodyparts2plot):
    df_likelihood[bpindex,:] = data[DLCscorer,bp,'likelihood'].values
    df_x[bpindex,:] = data[DLCscorer,bp,'x'].values # an array of 26 arrays with 5305 elements each
    df_y[bpindex,:] = data[DLCscorer,bp,'y'].values # an array of 26 arrays with 5305 elements each

colors = cm.rainbow(np.linspace(0, 1, len(bodyparts2plot)))
camera = Camera(plt.figure())

for frame in range(0, nframes):
    print("Processing Frame", frame)
    plt.scatter(df_x[:, frame], df_y[:, frame], c=colors, s=100) # how to handle low confidence
    
    plt.ylim(720, 0) # https://stackoverflow.com/questions/2051744/reverse-y-axis-in-pyplot

    m_midline = plot_regression(22, 26, frame)
    m_c1_l = plot_regression(0, 5, frame)
    m_c1_r = plot_regression(5, 10, frame)
    angle_l = np.degrees(np.arctan((m_midline - m_c1_l)/(1 + m_midline * m_c1_l)))
    angle_r = np.degrees(np.arctan((m_midline - m_c1_r)/(1 + m_midline * m_c1_r)))
    annotation = "Left Angle: " + str(np.around(angle_l)) + "Right Angle: " + str(np.around(angle_r))
    
    plt.text(0, 720, annotation)
    camera.snap()
print("Animating...")
anim = camera.animate(blit=True)
print("Saving...")
anim.save('scatter.mp4', fps=60)