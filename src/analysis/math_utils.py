import numpy as np
from scipy.spatial import distance as dist


def regress(df_x, df_y, df_likelihood, start_df_ind, end_df_ind, frame):
    # print("Regressing frame", frame)
    x_arr = df_x[start_df_ind:end_df_ind, frame]
    y_arr = df_y[start_df_ind:end_df_ind, frame]

    ind_to_delete = np.array([])

    for bp_index in range(start_df_ind, end_df_ind):
        if df_likelihood[bp_index, frame] < 0.99:
            ind_to_delete = np.append(ind_to_delete, bp_index - start_df_ind)
            # print("bp_index", bp_index, "(actual index:", bp_index - start_df_ind, ") tagged for deletion, where start bp index is", start_df_ind, "and end bp index is", end_df_ind)
            # print("ind_to_delete is now", ind_to_delete)

    if ind_to_delete.size != 0:
        x_arr = np.delete(x_arr, ind_to_delete)
        y_arr = np.delete(y_arr, ind_to_delete)
        # print("x_arr is now", x_arr)
        # print("y_arr is now", y_arr)

    # If there is less than 3 points that we're confident about, return None
    if len(x_arr) < 3:
        return None

    m, b = np.polyfit(x_arr, y_arr, 1)
    return (m, b)


def distance(df_x, df_y, df_ind_1, df_ind_2, frame):
    p1 = (df_x[df_ind_1, frame], df_y[df_ind_1, frame])
    p2 = (df_x[df_ind_2, frame], df_y[df_ind_2, frame])

    d = dist.euclidean(p1, p2)
    return d

def interpolate_gaps(values, limit=None):
    """
    Fill gaps using linear interpolation, optionally only fill gaps up to a
    size of `limit`.
    https://stackoverflow.com/questions/36455083/working-with-nan-values-in-matplotlib
    """
    values = np.asarray(values)
    i = np.arange(values.size)
    valid = np.isfinite(values)
    filled = np.interp(i, i[valid], values[valid])

    if limit is not None:
        invalid = ~valid
        for n in range(1, limit + 1):
            invalid[:-n] &= invalid[n:]
        filled[invalid] = np.nan

    return filled