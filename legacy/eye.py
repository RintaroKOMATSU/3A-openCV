import numpy as np

THRESHOLD_CLOSE = 0.17
THRESHOLD_OPEN = 0.18


def eval_eye_close(face_landmarks, eye_landmarks, threshold_close = THRESHOLD_CLOSE, threshold_open = THRESHOLD_OPEN):
    eye_points = np.array([[face_landmarks.landmark[i].x, face_landmarks.landmark[i].y] for i in eye_landmarks])
    # EAR計算

    A = np.linalg.norm(eye_points[1] - eye_points[5])
    B = np.linalg.norm(eye_points[2] - eye_points[4])
    C = np.linalg.norm(eye_points[0] - eye_points[3])
    eye_ratio = (A + B) / (2.0 * C)
    if eye_ratio < threshold_close:
        return -1 #eye close
    elif eye_ratio > threshold_open:
        return 1 #eye open
    return 0

def eval_eye_stat(face_landmarks, left_eye_landmarks, right_eye_landmarks, threshold_close = THRESHOLD_CLOSE, threshold_open = THRESHOLD_OPEN):
    eye_stat = 0 #open both
    left_eye_stat = eval_eye_close(face_landmarks, left_eye_landmarks, threshold_close, threshold_open)
    right_eye_stat = eval_eye_close(face_landmarks, right_eye_landmarks, threshold_close, threshold_open)
    if (right_eye_stat == 1 and left_eye_stat == -1):
        eye_stat = -1 #left wink
    elif(right_eye_stat == -1 and left_eye_stat == 1):
        eye_stat = 1 #right wink
    elif(right_eye_stat == -1 and left_eye_stat == -1):
        eye_stat = 2 #close both
    return eye_stat
    
def cal_eye_ratio(face_landmarks, eye_landmarks):
    eye_points = np.array([[face_landmarks.landmark[i].x, face_landmarks.landmark[i].y] for i in eye_landmarks])
    # EAR計算

    A = np.linalg.norm(eye_points[1] - eye_points[5])
    B = np.linalg.norm(eye_points[2] - eye_points[4])
    C = np.linalg.norm(eye_points[0] - eye_points[3])
    eye_ratio = (A + B) / (2.0 * C)
    return eye_ratio
    
def print_eye_stat(eye_stat):
    if eye_stat == 0:
        print("open both", end = "\r")
    elif eye_stat == 1:
        print("left wink", end = "\r")
    elif eye_stat == -1:
        print("right wink", end = "\r")
    elif eye_stat == 2:
        print("close both", end = "\r")

