import os
from tqdm import tqdm

import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
import pandas as pd
import math_utils
import draw

from celluloid import Camera

class Analysis:
    def __init__(self, h5_path, DLCscorer, startframe=None, endframe=None):
        df = pd.read_hdf(h5_path)

        if startframe is None:
            self.startframe = 0
        else:
            self.startframe = startframe
        if endframe is None:
            self.endframe = len(df.index)
        else:
            self.endframe = endframe
        assert self.endframe > self.startframe
        self.nframes = self.endframe - self.startframe

        self.bodyparts2plot = list(df.columns.levels[1])
        self.df_likelihood = np.empty((len(self.bodyparts2plot), self.nframes))
        self.df_x = np.empty((len(self.bodyparts2plot), self.nframes))
        self.df_y = np.empty((len(self.bodyparts2plot), self.nframes))

        self.outpath = os.path.dirname(
            os.path.abspath(__file__)) + '\\output\\'

        self.datastore = dict()

        for bpindex, bp in enumerate(self.bodyparts2plot):
            self.df_likelihood[bpindex, :] = \
                df[DLCscorer, bp, 'likelihood'].values[startframe:endframe]
            self.df_x[bpindex, :] = \
                df[DLCscorer, bp, 'x'].values[startframe:endframe]
            self.df_y[bpindex, :] = \
                df[DLCscorer, bp, 'y'].values[startframe:endframe]
    
    def duplicate_name_check(self, name):
        if name in self.datastore:
            raise RuntimeError(name + " has already been defined.")

    def calc_regression_line(self, name, start_df_ind, end_df_ind):
        """
        Calculate the line obtained by regressing points between
        start_df_ind and end_df_ind.

        Parameters:
            name (str): identifier for the resulting line
            start_df_ind (int): the dataframe index of the first point to be 
                                included in the regression 
            end_df_ind (int): the dataframe index of the last point to be 
                              included in the regression 
        Returns:
            Nothing; adds the line identified by name to self.datastore
        """
        self.duplicate_name_check(name)
        m_arr = np.empty((self.nframes, 2))
        frames_w_invalid_reg = []

        print("\nStarting regressing line", name, 
                    "between df indices", start_df_ind, "and", end_df_ind)
        for frame in tqdm(range(self.startframe, self.endframe)):
            m = math_utils.regress(self.df_x, self.df_y,
                                   self.df_likelihood, 
                                   start_df_ind, end_df_ind, frame)
            if m is None:
                frames_w_invalid_reg.append(frame)
                m_arr[frame] = [np.nan, np.nan]
            else:
                m_arr[frame] = m
        self.datastore[name] = m_arr

        if len(frames_w_invalid_reg) > 0:
            print("Not enough points to regress on frames", 
                            str(frames_w_invalid_reg), "for", name)
        print("Successfully regressed line", name, 
                    "between df indices", start_df_ind, "and", end_df_ind)
    
    def calc_perpendicular_line(self, name, line_name):
        """
        Calculate the line perpendicular to another line.

        Parameters:
            name (str): identifier for the resulting line
            line_name (str): identifier for the input line
        
        Returns:
            Nothing; adds the line identified by name to self.datastore
        """
        self.duplicate_name_check(name)
        m_arr = np.empty((self.nframes, 2))
        m_old_arr = self.datastore[line_name]

        print("\nStarting calculating line", name, 
                    "which is perpendicular to", line_name)
        for frame in tqdm(range(self.startframe, self.endframe)):
            m = (1 / m_old_arr[frame][0], m_old_arr[frame][1])
            m_arr[frame] = m
        self.datastore[name] = m_arr
        print("Successfully calculated line", name, 
                    "which is perpendicular to", line_name)
    
    def calc_angle(self, name, line1_name, line2_name, fill_gaps=False):
        """
        Calculate the angle between two lines, where the angle from the
        first line to the second line is positive when clockwise and
        negative when counterclocksize. 

        Parameters:
            name (str): identifier for the resulting angle
            line1_name (str): identifier for the first line
            line2_name (str): identifier for the second line
            fill_gaps (bool): when set to True, intopolate gaps to fill in
                              NaN values
        
        Returns:
            Nothing; adds the angle identified by name to self.datastore
        """
        self.duplicate_name_check(name)
        angle_arr = np.empty(self.nframes)
        m1_arr = self.datastore[line1_name]
        m2_arr = self.datastore[line2_name]

        print("\nStarting calculating angle", name, 
                    "between lines", line1_name, "and", line2_name)
        for frame in tqdm(range(self.startframe, self.endframe)):
            if m1_arr[frame] is [np.nan, np.nan] or m2_arr[frame] is [np.nan, np.nan]:
                angle_arr[frame] = np.nan
            else:
                m1 = m1_arr[frame][0]
                m2 = m2_arr[frame][0]
                angle_arr[frame] = \
                        np.degrees(np.arctan((m1 - m2) / (1 + m1 * m2)))
        if fill_gaps == True:
            angle_arr = math_utils.interpolate_gaps(angle_arr)
        self.datastore[name] = angle_arr
        print("Successfully calculated angle", name, 
                    "between lines", line1_name, "and", line2_name)
    
    def calc_segment_len(self, name, df_ind_1, df_ind_2, fill_gaps=False):
        """
        Calculate the length of a segment formed by two points, specified
        by df_ind_1 and df_ind_2.

        Parameters:
            name (str): identifier for the resulting segment
            df_ind_1 (int): the dataframe index of the first point
                            that forms the segment
            df_ind_2 (int): the dataframe index of the second point 
                            that forms the segment
            fill_gaps (bool): when set to True, intopolate gaps to fill in
                              NaN values
        
        Returns:
            Nothing; adds the segment identified by name to self.datastore
        """
        self.duplicate_name_check(name) 
        seg_arr = np.empty(self.nframes)

        print("\nStarting calculating length of segment", name, 
              "between df indices", df_ind_1, "and", df_ind_2)
        for frame in tqdm(range(self.startframe, self.endframe)):
            seg_arr[frame] = math_utils.distance(self.df_x, self.df_y, 
                                                 df_ind_1, df_ind_2, frame)

        if fill_gaps == True:
            seg_arr = math_utils.interpolate_gaps(seg_arr)

        self.datastore[name] = seg_arr
        print("Successfully calculated length of segment", name, 
              "between df indices", df_ind_1, "and", df_ind_2)
    
    def calc_avg(self, name, data_name_arr):
        """
        Calculate the vectorized average of several pieces of data.

        Parameters:
            name (str): identifier for the resulting data
            data_name_arr (str list): list of identifiers for the input
                                      data
        
        Returns:
            Nothing; adds the data identified by name to self.datastore
        """
        print("\nStarting calculating", name, 
                "which is the average of", str(data_name_arr))
        data_stack = []
        for data_name in data_name_arr:
            data_stack.append(self.datastore[data_name])
        data_stack = np.vstack(tuple(data_stack))

        self.datastore[name] = np.average(data_stack, axis=0)
        print("Successfully calculated", name, 
                "which is the average of", str(data_name_arr))

    def plot(self, y_name, label, x_name='frame'):
        if x_name == 'frame':
            x = range(self.startframe, self.endframe)
        y = self.datastore[y_name][self.startframe:self.endframe]

        plt.plot(x, y, label=label)
    
    def save_plot(self, xlabel, ylabel):
        plt.xlabel(xlabel)
        plt.ylabel(ylabel)
        plt.show()

        outfile = self.outpath + xlabel + '_vs_' + ylabel
        plt.savefig(outfile, dpi=300)
        print(outfile, "saved!")
    
    def save_csv(self, col_names, filename):
        cols = []
        for name in col_names:
            cols.append(self.datastore[name].reshape(-1, 1))
        
        df = pd.DataFrame(np.hstack(tuple(cols)))
        df.columns = col_names
        
        df.to_csv(os.path.join(self.outpath, filename))


    # TODO: improve this abstraction
    # TODO: this currently errors with trying to annotate the perpendicular
    # line
    def annotate_video(self, videopath, line_names, angle_names):
        lines_to_draw = [self.datastore[name] for name in line_names]
        angles_to_print = {"left": self.datastore[angle_names[0]], 
                           "right": self.datastore[angle_names[1]]}
        
        draw.draw(path=videopath, 
                  startframe=self.startframe,
                  endframe=self.endframe,
                  lines=lines_to_draw, 
                  segments=None, 
                  angles=angles_to_print, 
                  outfile=self.outpath)

    # TODO: change this method to match new abstraction
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