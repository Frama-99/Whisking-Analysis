import deeplabcut
import yaml

DLC_CFG_PATH = r'C:\Users\Ma990\OneDrive - Tufts\Jobs\SPEL\Whisking\Whisking_with_Midline-Frank Ma-2020-05-03\config.yaml'

def do_dlc_inference(video, dest):
    deeplabcut.analyze_videos(config_path=DLC_CFG_PATH, videos=[video], save_as_csv=False, destfolder=dest)
    