#Captures face from webcam stream
#Removes background from the picture obtained from webcam stream
#Replaces and rescales in realtime the face obtained instead of the face of the video


import numpy as np
import cv2
import sys

cam = cv2.VideoCapture(0)
cascPath = sys.argv[1]
faceCascade = cv2.CascadeClassifier(cascPath)

cv2.namedWindow('captura',cv2.WINDOW_AUTOSIZE)
cv2.moveWindow('captura',100,100)

factor = 30


def end():
	cam.release()
	cv2.destroyAllWindows()

def obtainFace():
	while(True):
		ret,frame = cam.read()
		gray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)

		if ret==True:
			faces = faceCascade.detectMultiScale(
			gray,
			scaleFactor=1.1,
			minNeighbors=10,
			minSize=(30, 30),
			#flags = cv2.CASCADE_SCALE_IMAGE
			)
			#gfmask = fgbh.apply(gray)
			for (x,y,w,h) in faces:
				factor = int(0.15*w)
				cv2.rectangle(frame,((x-factor),(y-factor)),((x+factor)+w,(y+factor)+h),(0,255,0),2)
			cv2.imshow('captura',frame)
			
			k = cv2.waitKey(33)
			if k==27:
				end()
				break
			elif k==99:
				print("capture")
				cv2.imwrite('kk.png',frame[(y-factor):(y+factor+h),(x-factor):(x+factor+w)])
				cam.release()
				processFoto()
				break
		else:
			break
			

def processFoto():
	imgo = cv2.imread('kk.png',-1)
	height, width = imgo.shape[:2]
	print
	mask = np.zeros(imgo.shape[:2],np.uint8)

	bgdModel = np.zeros((1,65),np.float64)
	fgdModel = np.zeros((1,65),np.float64)

	factor = int(0.15*width)
	rect = (0,0,width-factor, height-factor)
	cv2.grabCut(imgo,mask,rect,bgdModel,fgdModel,5,cv2.GC_INIT_WITH_RECT)
	mask = np.where((mask==2)|(mask==0),0,1).astype('uint8')
	img1 = imgo *mask[:,:,np.newaxis]

	background = imgo-img1

	background[np.where((background>[0,0,0]).all(axis=2))] = [255,255,255]
	final = background+img1
	cv2.imwrite('kk2.png',final)
	processVideo()


def processVideo():
	print("processVideo()")
	careto = cv2.imread('kk2.png')	
	
	cam = cv2.VideoCapture('she.mp4')
	while(True):
		ret,frame = cam.read()

		gray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
	
		
		if ret==True:

			faces = faceCascade.detectMultiScale(
			gray,
			scaleFactor=1.1,
			minNeighbors=10,
			minSize=(30, 30),
			#flags = cv2.CASCADE_SCALE_IMAGE
			)
			#gfmask = fgbh.apply(gray)
			for (x,y,w,h) in faces:
				factor = int(0.15*w)
				putMask(careto,frame,x-factor,y-factor,w+factor,h+factor)
				orig_mask = careto[:,:,2]
				orig_mask_inv = cv2.bitwise_not(orig_mask)
				img_careto = careto[:,:,0:3]
				height,width = img_careto.shape[:2]
				roi_gray = gray[y:y+h,x:x+w]
				roi_color = frame[y:y+h,x:x+w]
				#cv2.imwrite('kk.jpg',frame[x-factor:(x+factor+w),y-factor:(y+factor+h)])
				cv2.rectangle(frame,((x-factor),(y-factor)),((x+factor)+w,(y+factor)+h),(0,255,0),2)
				
			#cv2.add(careto,1.0,output,0.5,0,output)
			cv2.imshow('captura',frame)
			k = cv2.waitKey(33)
		
			if k==27:
				end()
				break	
		else:
			break
	cam.release()
def putMask(img,src,x,y,w,h):
	
	
	print("putmask")
	
	img_res = cv2.resize(img, (w, h))
	cv2.imwrite('kk_resize.jpg',img_res)

	rows,cols,channels = img_res.shape
	roi = img_res[0:rows,0:cols]
	roi_src = src[y:(y+rows),x:(x+cols)]


	
	img2gray = cv2.cvtColor(img_res,cv2.COLOR_BGR2GRAY)
	
	ret, mask = cv2.threshold(img2gray,190,255,cv2.THRESH_BINARY_INV)
	print(ret)
	
	mask_inv = cv2.bitwise_not(mask)
	
	
	#blackout the area of ROI
	src_bg=cv2.bitwise_and(roi_src,roi_src,mask=mask_inv)
		
	#Take only the ROI of img
	img_fg = cv2.bitwise_and(img_res,img_res,mask=mask)
		
	#put new face in ROI
	dst = cv2.add(src_bg,img_fg)
	rows,cols,channels = dst.shape
	src[y:(y+rows),x:(x+cols)]=dst
	
	return
#obtainFace()
processFoto()
processVideo()
		

