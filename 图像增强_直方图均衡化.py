from cv2 import *
import numpy as np
import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False
imgName = input("请输入一副待处理图片,或直接将图片拖入窗口并点击窗口按下回车键\n")
while (imgName != "exit"):
    im = imdecode(np.fromfile(imgName, dtype=np.uint8),1)
    #im=imread(imgName)
    imshow(imgName + "origin", im)
    im_grey = cvtColor(im, COLOR_BGR2GRAY)
    hist = calcHist([im_grey], [0], None, [256], [0, 255])
    hist = hist.squeeze() / hist.sum()

 #   plt.figure(1)
 #   plt.bar(range(len(hist)), hist, fc='black')
 #   plt.title(imgName + "原图灰度直方图")
 #   plt.xlabel("灰度")
 #   plt.ylabel("频率")
 #   plt.show()

    print(im.shape)
    im_fin = np.zeros((im.shape), dtype=np.uint8)
    im_fix = np.zeros((im.shape), dtype=np.uint8)
    for k in range(3):
        hist = calcHist([im], [k], None, [256], [0, 255])
        hist = hist.squeeze() / hist.sum()
        v = np.zeros((1, 257), dtype=float)
        for i in range(im_grey.shape[0]):
            for j in range(im_grey.shape[1]):
                im_fix[i][j][k] = np.uint8(255 * hist[0:im[i][j][k]].sum())
        beta = 0.1

        for i in range(256):
            v[0][i + 1] = (1 - beta) * v[0][i] + beta * hist[i]

        v = v.squeeze()
        for i in range(256):
            v[i + 1] = v[i + 1] / (1 - np.power(0.9, i + 1))
        v = v / v.sum()
        # 用指数加权平均
        # 用直方图均衡化处理图片0
        for i in range(im_grey.shape[0]):
            for j in range(im_grey.shape[1]):
                im_fin[i][j][k] = np.uint8(255 * v[1:im[i][j][k]].sum())
    imshow(imgName + " fix", im_fix)
    imshow(imgName + " fixed_epo", im_fin)

    im_grey = cvtColor(im_fin, COLOR_BGR2GRAY)

 #   hist = calcHist([im_grey], [0], None, [256], [0, 255])
 #   hist = hist.squeeze() / hist.sum()
#
 #   plt.figure(2)
 #   plt.bar(range(len(hist)), hist, fc='black')
 #   plt.title(imgName + " 用指数加权平均的直方图函数做均衡")
 #   plt.xlabel("灰度")
 #  # plt.ylabel("频率")
 #   plt.show()

    waitKey()
    imgName = input("请输入另一幅图片,或直接将图片拖入窗口并点击窗口按下回车键\n")
