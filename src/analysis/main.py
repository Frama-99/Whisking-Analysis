from analysis import Analysis
from draw import draw
import argparse
import numpy as np

DEBUG = True

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-V',
                        '--videopath',
                        help='Path to the video file or directory',
                        dest='videopath',
                        required=False,
                        metavar='VIDEODIR')
    parser.add_argument('-H',
                        '--h5path',
                        help='Path to the h5 file',
                        dest='h5path',
                        required=False,
                        metavar='H5DIR')
    parser.add_argument('output_dir',
                        help='Output Directory of the json files',
                        nargs='?',
                        metavar='OUTDIR')
    args = parser.parse_args()

    if args.h5path is not None:
        h5path = args.h5path
    if args.videopath is not None:
        videopath = args.videopath
    if args.h5path is None and args.videopath is None:
        if DEBUG == True:
            print("Setting h5path and videopath to defaults in DEBUG")
            h5path = (r'C:\Users\Ma990\Code\spel\whisking\sample-project'
                   r'\Whisking_with_Midline-Frank Ma-2020-05-03\videos'
                   r'\iteration-2\6400ratDLC_resnet50_Whisking_with_Midline'
                   r'May3shuffle1_500000.h5')
            videopath = (r'C:\Users\Ma990\Code\spel\whisking\sample-project'
                     r'\Whisking_with_Midline-Frank Ma-2020-05-03\videos'
                     r'\iteration-2\6400ratDLC_resnet50_Whisking_with_Midline'
                     r'May3shuffle1_500000_labeled.mp4')
        else:
            raise RuntimeError("""You need to provide either a video directory 
                        or the DLC generated .h5 file.""")

    DLCscorer = 'DLC_resnet50_Whisking_with_MidlineMay3shuffle1_500000'
    
    analysis = Analysis(h5_path=h5path, 
                        DLCscorer=DLCscorer, 
                        startframe=None, 
                        endframe=None)

    analysis.calc_regression_line('midline', 22, 25)
    analysis.calc_perpendicular_line('perp_to_midline', 'midline')
    analysis.calc_regression_line('left_c1', 0, 5)
    analysis.calc_regression_line('right_c1', 5, 10)
    analysis.calc_angle('left_c1_angle', 'perp_to_midline', 'left_c1')
    analysis.calc_angle('right_c1_angle', 'perp_to_midline', 'right_c1')
    analysis.plot('left_c1_angle', 'Left C1 Angle')
    analysis.plot('right_c1_angle', 'Right C1 Angle')
    analysis.save_plot('frame', 'Whisker Angles')
    analysis.save_csv(['left_c1_angle', 'right_c1_angle'], 'angles.csv')

    analysis.calc_segment_len('left_lid_1', 11, 15)
    analysis.calc_segment_len('left_lid_2', 12, 14)
    analysis.calc_avg('left_lid_avg', ['left_lid_1', 'left_lid_2'])
    analysis.calc_segment_len('right_lid_1', 17, 21)
    analysis.calc_segment_len('right_lid_2', 18, 20)
    analysis.calc_avg('right_lid_avg', ['right_lid_1', 'right_lid_2'])
    analysis.plot('left_lid_avg', 'Left Blink Signal')
    analysis.plot('right_lid_avg', 'Right Blink Signal')
    analysis.save_plot('frame', 'Blinking Signal')
    analysis.save_csv(['left_lid_avg', 'left_lid_avg'], 'blink.csv')

    # TODO: change these to match new abstraction
    # analysis.annotate_video(videopath=videopath)
    # analysis.animate(bp='eyes', fps=239.76)
    # analysis.animate(bp='whiskers', fps=239.76)

if __name__ == '__main__':
    main()