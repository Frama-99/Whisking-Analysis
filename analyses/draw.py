import cv2
import os
import sys
import numpy as np

import time
from tqdm import tqdm
"""
Reference: https://github.com/nadamian/VCTrack/blob/master/draw.py
"""


def find_line_start_end(m, b, w, h):
    """
    The goal here is to find points at the edges of the frame that can
    serve as the start and end point of our line. This function takes in
    the slope and y intercept of the line and the frame size and returns
    the coordinate of thw two points of intersection.

    Given a line, there are four possible points it can intersect with
    a rectangle that is the size of our frame. They are:
    - the y intercept: (0, b)
    - the x intercept: (-b/m, 0)
    - intersection between the line and x = w: (w, mw + b)
    - intersection between the line and y = h: ((h - b)/m, h)

    Only two of these points of intersections should be present at a time.
    Meaning that only two of the following four conditions will be true at
    one time.
    - 0 <= b <= h
    - 0 <= -b/m <= w
    - 0 <= mw + b <= h
    - 0 <= (h - b)/m <= w
    """
    intersect = []
    # print("m =", m, "b =", b)
    if np.isnan(m) or np.isnan(b):
        return None

    if b >= 0 and b <= h:
        intersect.append((0, b))
    if (-b / m) >= 0 and (-b / m) <= w:
        if (-b / m, 0) not in intersect:
            intersect.append((-b / m, 0))
    if (m * w + b) >= 0 and (m * w + b) <= h:
        if (w, m * w + b) not in intersect:
            intersect.append((w, m * w + b))
    if (h - b) / m >= 0 and (h - b) / m <= w:
        if ((h - b) / m, h) not in intersect:
            intersect.append(((h - b) / m, h))
    if len(intersect) is not 2:
        raise RuntimeError(
            "Unknown line end points calculation error. Intersect = " +
            str(intersect) + ", m = " + str(m) + ", b = " + str(b))
    return [(int(round(i[0])), int(round(i[1]))) for i in intersect]


def draw(path, startframe, endframe, lines, angles, outfile, videotype='.mp4'):
    """Takes each frame from video and stitches it back into new video with
    line drawn on."""

    # getting video parameters
    cap = cv2.VideoCapture(path)
    fps = cap.get(cv2.CAP_PROP_FPS)
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

    # make sure that the first frame read will be the start frame
    cap.set(cv2.CAP_PROP_POS_FRAMES, startframe - 1)
    # s signals whether or not we are at the end, im is the frame
    s, im = cap.read()
    # count for which frame we are on
    frame = startframe

    # specify output video format and output path
    name = path[path.rfind('\\') + 1:path.rfind('.')] + 'with_lines'
    if videotype == '.mp4':
        fourcc = cv2.VideoWriter.fourcc('m', 'p', '4', 'v')
    elif videotype == '.avi':
        fourcc = cv2.VideoWriter.fourcc('x', 'v', 'i', 'd')
    else:
        fourcc = 0

    # w is the video object to be outputted
    print("video output path:", os.path.join(outfile, name + videotype))
    w = cv2.VideoWriter(os.path.join(outfile, name + videotype), \
                        fourcc, fps, (width, height))

    print('Printing lines on your video.')
    frames = range(startframe, endframe)
    
    # loop until we're at the endframe or at the end of the video
    for frame in tqdm(frames):
        if not s:
            print("The provided endframe {endframe} is larger than the number of available frames, {frame}. Stopping.")
            break

        # print lines
        for line in lines:
            line_ends = find_line_start_end(*line[frame], width, height)
            if line_ends is not None:
                cv2.imwrite(path + '.png',
                            cv2.line(im, *line_ends, (255, 0, 0), 2))

        # print angle numbers
        angle_annotation = "Left Angle: " + str(
            np.around(angles['left'][frame], 2)) + " Right Angle: " + str(
                np.around(angles['right'][frame], 2))
        cv2.putText(img=im,
                    text=angle_annotation,
                    org=(10, 50),
                    fontFace=cv2.FONT_HERSHEY_SIMPLEX,
                    fontScale=1,
                    color=(255, 255, 255),
                    thickness=2,
                    lineType=cv2.LINE_AA)

        w.write(im)
        s, im = cap.read()

    # cleanup
    cap.release()
    w.release()
    cv2.destroyAllWindows()