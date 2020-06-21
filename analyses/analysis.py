import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
import pandas as pd

from celluloid import Camera
import os
from draw import draw
import math_utils


class Analysis:
    def __init__(self, h5_path, DLCscorer):
        df = pd.read_hdf(h5_path)
        # https://stackoverflow.com/questions/34082137/how-to-get-pandas-column-multiindex-names-as-a-list
        self.bodyparts2plot = list(df.columns.levels[1])
        self.nframes = len(df.index)
        self.df_likelihood = np.empty((len(self.bodyparts2plot), self.nframes))
        self.df_x = np.empty((len(self.bodyparts2plot), self.nframes))
        self.df_y = np.empty((len(self.bodyparts2plot), self.nframes))
        self.outpath = os.path.dirname(
            os.path.abspath(__file__)) + '\\output\\'

        for bpindex, bp in enumerate(self.bodyparts2plot):
            self.df_likelihood[bpindex, :] = df[DLCscorer, bp,
                                                'likelihood'].values
            # an array of 26 arrays with 5305 elements each
            self.df_x[bpindex, :] = df[DLCscorer, bp, 'x'].values
            # an array of 26 arrays with 5305 elements each
            self.df_y[bpindex, :] = df[DLCscorer, bp, 'y'].values

    def plot_whisker_angles(self,
                            startframe,
                            endframe,
                            fill_gaps=False,
                            draw_video=False,
                            animate=False,
                            fps=60):
        self.m_midline_arr = np.empty((self.nframes, 2))
        self.m_c1_l_arr = np.empty((self.nframes, 2))
        self.m_c1_r_arr = np.empty((self.nframes, 2))
        self.angle_l_arr = np.empty(self.nframes)
        self.angle_r_arr = np.empty(self.nframes)

        for frame in range(startframe, endframe):
            if frame % 100 == 0:
                print("Processing Frame", frame)

            angle_nan = False

            m_midline = math_utils.regress(self.df_x, self.df_y,
                                           self.df_likelihood, 22, 25, frame)
            if m_midline == None:
                print("Not enough points to regress on frame", frame,
                      "for midline.")
                self.m_midline_arr[frame] = [np.nan, np.nan]
                angle_nan = True
            else:
                self.m_midline_arr[frame] = m_midline

            m_c1_l = math_utils.regress(self.df_x, self.df_y,
                                        self.df_likelihood, 0, 5, frame)
            if m_c1_l == None:
                print("Not enough points to regress on frame", frame,
                      "for left c1.")
                self.m_c1_l_arr[frame] = [np.nan, np.nan]
                angle_nan = True
            else:
                self.m_c1_l_arr[frame] = m_c1_l

            m_c1_r = math_utils.regress(self.df_x, self.df_y,
                                        self.df_likelihood, 5, 10, frame)
            if m_c1_r == None:
                print("Not enough points to regress on frame", frame,
                      "for right c1.")
                self.m_c1_r_arr[frame] = [np.nan, np.nan]
                angle_nan = True
            else:
                self.m_c1_r_arr[frame] = m_c1_r

            if angle_nan == True:
                self.angle_l_arr[frame] = np.nan
                self.angle_r_arr[frame] = np.nan
            else:
                # -90 because we are defining the Whisker angle as
                # retraction = negative and protraction = positive, with
                # the line perpendicular to the midline being 0.
                angle_l = np.degrees(
                    np.arctan((m_midline[0] - m_c1_l[0]) /
                              (1 + m_midline[0] * m_c1_l[0])))
                angle_r = np.degrees(
                    np.arctan((m_midline[0] - m_c1_r[0]) /
                              (1 + m_midline[0] * m_c1_r[0])))
                self.angle_l_arr[frame] = angle_l
                self.angle_r_arr[frame] = angle_r

        if fill_gaps == True:
            self.angle_l_arr = math_utils.interpolate_gaps(self.angle_l_arr)
            self.angle_r_arr = math_utils.interpolate_gaps(self.angle_r_arr)

        plt.plot(range(startframe, endframe),
                 self.angle_l_arr[startframe:endframe],
                 label="Left C1 Angle")
        plt.plot(range(startframe, endframe),
                 self.angle_r_arr[startframe:endframe],
                 label="Right C1 Angle")
        plt.xlabel('frame')
        plt.ylabel('Angle in degrees')
        plt.show()

        plt.savefig(self.outpath + 'whisker_angles.png', dpi=300)
        print("whisker_angles.png saved!")

        if animate == True:
            self.animate(startframe, endframe, bp='whiskers', fps=fps)

    def plot_blink_signal(self,
                          startframe,
                          endframe,
                          fill_gaps=False,
                          animate=False,
                          fps=60):
        self.d_l_arr = np.empty(self.nframes)
        self.d_r_arr = np.empty(self.nframes)

        for frame in range(startframe, endframe):
            if frame % 100 == 0:
                print("Processing Frame", frame)

            # Distance between upper and lower lids
            self.d_l_arr[frame] = (
                math_utils.distance(self.df_x, self.df_y, 11, 15, frame) +
                math_utils.distance(self.df_x, self.df_y, 12, 14, frame)) / 2
            self.d_r_arr[frame] = (
                math_utils.distance(self.df_x, self.df_y, 17, 21, frame) +
                math_utils.distance(self.df_x, self.df_y, 18, 20, frame)) / 2

        if fill_gaps == True:
            self.d_l_arr = math_utils.interpolate_gaps(self.d_l_arr)
            self.d_r_arr = math_utils.interpolate_gaps(self.d_r_arr)

        plt.plot(range(startframe, endframe),
                 self.d_l_arr[startframe:endframe],
                 label="Left Blink Signal")
        plt.plot(range(startframe, endframe),
                 self.d_r_arr[startframe:endframe],
                 label="Right Blink Signal")
        plt.xlabel('frame')
        plt.ylabel('Distance between upper and lower lids')
        # plt.show()
        plt.savefig(self.outpath + 'blink_signal.png', dpi=300)
        print("blink_signal.png saved!")

        if animate == True:
            self.animate(startframe, endframe, bp='eyes', fps=fps)

    def animate(self, startframe, endframe, bp=None, fps=60):
        colors = cm.rainbow(np.linspace(0, 1, len(self.bodyparts2plot)))
        camera = Camera(plt.figure())

        for frame in range(startframe, endframe):
            if frame % 100 == 0:
                print("Animating Frame", frame)
            plt.scatter(self.df_x[:, frame],
                        self.df_y[:, frame],
                        c=colors,
                        s=100)  # how to handle low confidence

            for bp_index, bp_likelihood in enumerate(
                    self.df_likelihood[:, frame]):
                plt.annotate(
                    np.around(bp_likelihood, 2),
                    (self.df_x[bp_index, frame], self.df_y[bp_index, frame]))

            # https://stackoverflow.com/questions/2051744/reverse-y-axis-in-pyplot
            plt.ylim(720, 0)

            if bp == 'whiskers':
                self.plot_regres(*self.m_midline_arr[frame])
                self.plot_regres(*self.m_c1_l_arr[frame])
                self.plot_regres(*self.m_c1_r_arr[frame])

                angle_annotation = "Left Angle: " + str(
                    np.around(self.angle_l_arr[frame],
                              2)) + " Right Angle: " + str(
                                  np.around(self.angle_r_arr[frame], 2))
                plt.text(650, 0, angle_annotation)

            if bp == 'eyes':
                x1_l = [self.df_x[11, frame], self.df_x[15, frame]]
                y1_l = [self.df_y[11, frame], self.df_y[15, frame]]
                x2_l = [self.df_x[12, frame], self.df_x[14, frame]]
                y2_l = [self.df_y[12, frame], self.df_y[14, frame]]
                x1_r = [self.df_x[17, frame], self.df_x[21, frame]]
                y1_r = [self.df_y[17, frame], self.df_y[21, frame]]
                x2_r = [self.df_x[18, frame], self.df_x[20, frame]]
                y2_r = [self.df_y[18, frame], self.df_y[20, frame]]
                plt.plot(x1_l, y1_l, color='blue')
                plt.plot(x1_r, y1_r, color='blue')
                plt.plot(x2_l, y2_l, color='blue')
                plt.plot(x2_r, y2_r, color='blue')

                blink_annotation = "Left Blink Signal: " + str(
                    np.around(self.d_l_arr[frame],
                              2)) + " Right Blink Signal: " + str(
                                  np.around(self.d_r_arr[frame], 2))
                plt.text(400, 0, blink_annotation)

            frame_annotation = "Frame: " + str(frame)
            plt.text(0, 0, frame_annotation)

            camera.snap()

        print("Animating...")
        anim = camera.animate(blit=True)
        print("Saving...")
        anim_filename = 'animation_' + bp + str(fps) + "fps.mp4"
        anim.save(self.outpath + anim_filename, fps=fps)

    def plot_regres(self, m, b):
        x = np.linspace(0, 1280, 100)
        y = m * x + b
        plt.plot(x, y, color='red')


def main():
    h5_path = r'C:\Users\Ma990\OneDrive - Tufts\Jobs\SPEL\Whisking\Whisking_with_Midline-Frank Ma-2020-05-03\videos\iteration-2\6400ratDLC_resnet50_Whisking_with_MidlineMay3shuffle1_500000.h5'
    DLCscorer = 'DLC_resnet50_Whisking_with_MidlineMay3shuffle1_500000'
    analysis = Analysis(h5_path, DLCscorer)
    analysis.plot_whisker_angles(1000,
                                 2000,
                                 fill_gaps=False,
                                 animate=False,
                                 fps=239.76)

    videopath = r'C:\Users\Ma990\OneDrive - Tufts\Jobs\SPEL\Whisking\Whisking_with_Midline-Frank Ma-2020-05-03\videos\iteration-2\6400ratDLC_resnet50_Whisking_with_MidlineMay3shuffle1_500000_labeled.MP4'
    lines_to_draw = [
        analysis.m_midline_arr, analysis.m_c1_l_arr, analysis.m_c1_r_arr
    ]
    angles_to_print = [analysis.angle_l_arr, analysis.angle_r_arr]
    outfile = analysis.outpath
    print(draw(videopath, 1000, 2000, lines_to_draw, angles_to_print, outfile))
    # analysis.plot_blink_signal(0, 5305, fill_gaps=False, animate=True, fps=239.76)


if __name__ == '__main__':
    main()