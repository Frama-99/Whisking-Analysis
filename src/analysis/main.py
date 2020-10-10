from analysis import Analysis
from draw import draw

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-v',
                        '--videodir',
                        help='Path to the video file or directory',
                        dest='video_dir',
                        required=False,
                        metavar='VIDEODIR')
    parser.add_argument('-h',
                        '--h5dir',
                        help='Path to the h5 file',
                        dest='h5_dir',
                        required=False,
                        metavar='H5DIR')
    parser.add_argument('output_dir',
                        help='Output Directory of the json files',
                        nargs='?',
                        metavar='OUTDIR')
    args = parser.parse_args()
    
    if parser.h5_dir is not None:
        pass
    elif parser.video_dir is not None:
        pass
    else:
        raise RuntimeError("You need to provide either a video directory or the DLC generated .h5 file.")

    # h5_path = r'C:\Users\Ma990\OneDrive - Tufts\Jobs\SPEL\Whisking\Whisking_with_Midline-Frank Ma-2020-05-03\videos\iteration-2\6400ratDLC_resnet50_Whisking_with_MidlineMay3shuffle1_500000.h5'
    DLCscorer = 'DLC_resnet50_Whisking_with_MidlineMay3shuffle1_500000'
    analysis = Analysis(h5_path, DLCscorer)


    videopath = r'C:\Users\Ma990\OneDrive - Tufts\Jobs\SPEL\Whisking\Whisking_with_Midline-Frank Ma-2020-05-03\videos\iteration-2\6400ratDLC_resnet50_Whisking_with_MidlineMay3shuffle1_500000_labeled.MP4'
    
    analysis.plot_whisker_angles(1000,
                                 2000,
                                 fill_gaps=False,
                                 animate=False,
                                 fps=239.76)
    lines_to_draw = [
        analysis.m_midline_arr, analysis.m_c1_l_arr, analysis.m_c1_r_arr
    ]
    angles_to_print = {"left": analysis.angle_l_arr, "right": analysis.angle_r_arr}
    outfile = analysis.outpath
    draw(path=videopath, startframe=0, endframe=5305, lines=lines_to_draw, segments=None, angles=angles_to_print, outfile=outfile)
    
    # analysis.plot_blink_signal(0, 5305, fill_gaps=False, animate=False, fps=239.76)


if __name__ == '__main__':
    main()