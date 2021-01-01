import os

import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
import pandas as pd
import math_utils
import draw

from celluloid import Camera

class Analysis:
    def __init__(self, h5_path, DLCscorer, startframe, endframe):
        df = pd.read_hdf(h5_path)
        # https://stackoverflow.com/questions/34082137/how-to-get-pandas-column-multiindex-names-as-a-list
        self.bodyparts2plot = list(df.columns.levels[1])
        self.nframes = len(df.index)
        self.df_likelihood = np.empty((len(self.bodyparts2plot), self.nframes))
        self.df_x = np.empty((len(self.bodyparts2plot), self.nframes))
        self.df_y = np.empty((len(self.bodyparts2plot), self.nframes))
        self.outpath = os.path.dirname(
            os.path.abspath(__file__)) + '\\output\\'

        self.startframe = startframe
        self.endframe = endframe

        self.whisker_analysis_completed = False
        self.blink_analysis_completed = False

        self.regression_lines = dict()
        self.angles = dict()
        self.segments = dict()

        for bpindex, bp in enumerate(self.bodyparts2plot):
            self.df_likelihood[bpindex, :] = df[DLCscorer, bp,
                                                'likelihood'].values
            # an array of 26 arrays with 5305 elements each
            self.df_x[bpindex, :] = df[DLCscorer, bp, 'x'].values
            # an array of 26 arrays with 5305 elements each
            self.df_y[bpindex, :] = df[DLCscorer, bp, 'y'].values

    def calc_regression_line(self, name, start_df_ind, end_df_ind):
        m_arr = np.empty((self.nframes, 2))
        for frame in range(self.startframe, self.endframe):
            if frame % 1000 == 0:
                print("Processing Frame", frame)
            m = math_utils.regress(self.df_x, self.df_y,
                                   self.df_likelihood, 
                                   start_df_ind, end_df_ind, frame)
            if m is None:
                print("Not enough points to regress on frame", frame, "for", name)
                m_arr[frame] = [np.nan, np.nan]
            else:
                m_arr[frame] = m
        self.regression_lines[name] = m_arr
        print("Successfully regressed line", name)
    
    def calc_perpendicular_line(self, name, line_name):
        m_arr = np.empty((self.nframes, 2))
        m_old_arr = self.regression_lines[line_name]
        for frame in range(self.startframe, self.endframe):
            if frame % 1000 == 0:
                print("Processing Frame", frame)
            m = (1 / m_old_arr[frame][0], m_old_arr[frame][1])
            m_arr[frame] = m
        self.regression_lines[name] = m_arr
        print("Successfully calculated", name, "which is perpendicular to", line_name)
    
    def calc_angle(self, name, line1_name, line2_name, fill_gaps=False):
        angle_arr = np.empty(self.nframes)
        m1_arr = self.regression_lines[line1_name]
        m2_arr = self.regression_lines[line2_name]
        for frame in range(self.startframe, self.endframe):
            if frame % 1000 == 0:
                print("Processing Frame", frame)
            if m1_arr[frame] is [np.nan, np.nan] or m2_arr[frame] is [np.nan, np.nan]:
                angle_arr[frame] = np.nan
            else:
                m1 = m1_arr[frame][0]
                m2 = m2_arr[frame][0]
                angle_arr[frame] = \
                        np.degrees(np.arctan(np.abs((m1 - m2) / (1 + m1 * m2))))
        if fill_gaps == True:
            angle_arr = math_utils.interpolate_gaps(angle_arr)
        self.angles[name] = angle_arr

    def plot(self, y_name, label, x_name='frame'):
        if x_name == 'frame':
            x = range(self.startframe, self.endframe)
        y = self.angles[y_name][self.startframe:self.endframe] 
        # TODO: expand to search over all self properties

        plt.plot(x, y, label=label)
    
    def save_plot(self, xlabel, ylabel):
        plt.xlabel(xlabel)
        plt.ylabel(ylabel)
        plt.show()

        outfile = self.outpath + xlabel + '_vs_' + ylabel
        plt.savefig(outfile, dpi=300)
        print(outfile, "saved!")


    def calc_whisker_angles(self, fill_gaps=False):
        self.m_midline_arr = np.empty((self.nframes, 2))
        self.m_c1_l_arr = np.empty((self.nframes, 2))
        self.m_c1_r_arr = np.empty((self.nframes, 2))
        self.angle_l_arr = np.empty(self.nframes)
        self.angle_r_arr = np.empty(self.nframes)

        for frame in range(self.startframe, self.endframe):
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
                              (1 + m_midline[0] * m_c1_l[0]))) + 90
                angle_r = 90 - np.degrees(
                    np.arctan((m_midline[0] - m_c1_r[0]) /
                              (1 + m_midline[0] * m_c1_r[0])))
                self.angle_l_arr[frame] = angle_l
                self.angle_r_arr[frame] = angle_r

        if fill_gaps == True:
            self.angle_l_arr = math_utils.interpolate_gaps(self.angle_l_arr)
            self.angle_r_arr = math_utils.interpolate_gaps(self.angle_r_arr)

        self.whisker_analysis_completed = True

    def plot_whisker_angles(self):
        plt.plot(range(self.startframe, self.endframe),
                 self.angle_l_arr[self.startframe:self.endframe],
                 label="Left C1 Angle")
        plt.plot(range(self.startframe, self.endframe),
                 self.angle_r_arr[self.startframe:self.endframe],
                 label="Right C1 Angle")
        plt.xlabel('frame')
        plt.ylabel('Angle in degrees')
        plt.show()

        plt.savefig(self.outpath + 'whisker_angles.png', dpi=300)
        print("whisker_angles.png saved!")

    def calc_blink_signal(self, fill_gaps=False):
        self.d_l_arr = np.empty(self.nframes)
        self.d_r_arr = np.empty(self.nframes)

        for frame in range(self.startframe, self.endframe):
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

        self.blink_analysis_completed = True

    def plot_blink_signal(self):
        plt.plot(range(self.startframe, self.endframe),
                 self.d_l_arr[self.startframe:self.endframe],
                 label="Left Blink Signal")
        plt.plot(range(self.startframe, self.endframe),
                 self.d_r_arr[self.startframe:self.endframe],
                 label="Right Blink Signal")
        plt.xlabel('frame')
        plt.ylabel('Distance between upper and lower lids')
        plt.show()
        plt.savefig(self.outpath + 'blink_signal.png', dpi=300)
        print("blink_signal.png saved!")

    def savecsv(self):
        if self.whisker_analysis_completed:
            whisker_angles = np.hstack((self.angle_l_arr.reshape(-1, 1), 
                                        self.angle_r_arr.reshape(-1, 1)))
            np.savetxt(self.outpath + 'whisker_angles.csv', 
                       whisker_angles, delimiter=',')
            print(self.outpath + 'whisker_angles.csv' + " saved!")

        if self.blink_analysis_completed:
            blink_signal = np.hstack((self.d_l_arr.reshape(-1, 1), 
                                      self.d_r_arr.reshape(-1, 1)))
            
            np.savetxt(self.outpath + 'blink_signal.csv', 
                       blink_signal, delimiter=',')
            print(self.outpath + 'whisker_angles.csv' + " saved!")

    def annotate_video(self, videopath):
        lines_to_draw = [
            self.m_midline_arr, self.m_c1_l_arr, self.m_c1_r_arr
        ]
        angles_to_print = {"left": self.angle_l_arr, 
                           "right": self.angle_r_arr}
        
        draw.draw(path=videopath, 
                  startframe=self.startframe,
                  endframe=self.endframe,
                  lines=lines_to_draw, 
                  segments=None, 
                  angles=angles_to_print, 
                  outfile=self.outpath)


    def animate(self, bp=None, fps=60):
        colors = cm.rainbow(np.linspace(0, 1, len(self.bodyparts2plot)))
        camera = Camera(plt.figure())

        for frame in range(self.startframe, self.endframe):
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