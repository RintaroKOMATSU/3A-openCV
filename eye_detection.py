import cv2
import mediapipe as mp
import numpy as np
import os
from multiprocessing import shared_memory, Lock

#camera setting
FRAME_WIDTH = 640
FRAME_HEIGHT=  480
FRAME_RATE = 60
  
#initialize mediapipe face mesh
mp_face_mesh = mp.solutions.face_mesh
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
drawing_spec = mp_drawing.DrawingSpec(thickness = 1, circle_radius = 1)

#initialize camera
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FPS, FRAME_RATE)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, FRAME_WIDTH)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT)

#initialize shared memory
shm_name = "landmarks"
lock= Lock()
shared_data = np.zeros(24)
shm = shared_memory.SharedMemory(name = shm_name, create = True, size = shared_data.nbytes)
buffer = np.ndarray(shared_data.shape, dtype=shared_data.dtype, buffer=shm.buf)
#set eye landmarks
right_eye_landmarks = [263, 386, 374, 362, 380, 373]
left_eye_landmarks = [33, 159, 145, 133, 153, 144]

#loop
with mp_face_mesh.FaceMesh(max_num_faces = 1,
                           refine_landmarks = True, 
                           min_detection_confidence = 0.5, 
                           min_tracking_confidence = 0.5) as face_mesh:
    while cap.isOpened():
        success, image = cap.read()
        if not success:
            print("ignoring empty camera frame.")
            continue
        image.flags.writeable = False
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        results = face_mesh.process(image)  
        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)


        if results.multi_face_landmarks:
            face_landmarks = results.multi_face_landmarks[0]
            for idx, landmark_idx in enumerate(right_eye_landmarks):
                shared_data[2*idx] = face_landmarks.landmark[landmark_idx].x
                shared_data[2*idx+1] = face_landmarks.landmark[landmark_idx].y
            for idx, landmark_idx in enumerate(left_eye_landmarks):
                shared_data[2*idx+12] = face_landmarks.landmark[landmark_idx].x
                shared_data[2*idx+13] = face_landmarks.landmark[landmark_idx].y

            lock.acquire()
            buffer[:] = shared_data[:]
            lock.release()

            mp_drawing.draw_landmarks(
                image=image,
                landmark_list=face_landmarks,
                connections=mp_face_mesh.FACEMESH_CONTOURS,
                landmark_drawing_spec=None,
                connection_drawing_spec=mp_drawing_styles
                .get_default_face_mesh_contours_style())

        
        cv2.imshow('MediaPipe Face Mesh', cv2.flip(image, 1))
        if cv2.waitKey(5) & 0xFF == 27:
            break
shm.close()
shm.unlink()
        
cap.release()




