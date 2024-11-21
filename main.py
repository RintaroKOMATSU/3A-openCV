import subprocess


#eye_detection from media pipe
subprocess.Popen(["python3", "src/eye_detection.py"])
#game process
subprocess.Popen(["bin/main"])
