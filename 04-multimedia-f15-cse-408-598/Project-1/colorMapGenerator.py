import math
import sys
import numpy as np
import cv2

# provides the hint message to user for selecting within the allowed range of possible values
def getComponentRange(choice):
    if choice == 1:
        return "R:(0,255) G:(0,255) B:(0,255)"

    #source: http://www.easyrgb.com/index.php?X=MATH&H=01#text1
    elif choice == 2:
        return "X:(0, 95.47) Y:(0, 100.000) Z:(0, 108.883)"

    elif choice == 3:
        return "L:(0, 100) a:(-128, 127) b:(-128, 127)"

    elif choice == 4:
        return "Y:(0, 255) U:(-128, 127) V:(-128, 127)"

    elif choice == 5:
       return "Y:(0,255) Cb:(0,255) Cr:(0,255)"

    elif choice == 6:
        return "Y:(0,255) I:(-128,127) Q:(-128,127)"

    elif choice == 7:
        return "H:(0,360) S:(0,1) L:(0,1)"

# provides the formatted value of color component based on color space
def getFormattedValue(value, c):
    if c == 1 or c == 4:
        return int(round(value));
    else:
        return "{:.2f}".format(value)


def convertLabToXYZComponents(L, a, b):

    var_Y1 = (L + 16) / 116;
    var_X1 = a / 500 + var_Y1;
    var_Z1 = var_Y1 - b / 200;

    if (var_Y1 ** 3 > 0.008856):
        var_Y1 = var_Y1 ** 3
    else:
        var_Y1 = (var_Y1 - 16 / 116) / 7.787

    if (var_X1 ** 3 > 0.008856):
        var_X1 = var_X1 ** 3
    else:
        var_X1 = (var_X1 - 16 / 116) / 7.787

    if (var_Z1 ** 3 > 0.008856):
        var_Z1 = var_Z1 ** 3
    else:
        var_Z1 = (var_Z1 - 16 / 116) / 7.787

    X = 95.047 * var_X1;
    Y = 100 * var_Y1;
    Z = 108.883 * var_Z1;

    return [X, Y, Z]

def convertLabToRGBComponents(L, a, b):
    #calcualte R G B from XYZ values. Hence, first convert Lab to XYZ
    C = convertLabToXYZComponents(L, a, b)
    return convertXYZToRGBComponents(C[0], C[1], C[2])

def convertHSLToRGBComponents(H, S, L):

    C = (1 - abs(2*L-1))*S;
    X = C * (1 - abs((H/60)%2-1))
    m = L - C/2

    if 0 <= H < 60 :
        R = C;
        G = X;
        B = 0;
    elif 60 <= H < 120 :
        R = X;
        G = C;
        B = 0;
    elif 120 <= H < 180 :
        R = 0;
        G = C;
        B = X;
    elif 180 <= H < 240 :
        R = 0;
        G = X;
        B = C;
    elif 240 <= H < 300 :
        R = X;
        G = 0;
        B = C;
    elif 300 <= H < 360 :
        R = C;
        G = 0;
        B = X;

    R = int(round((R + m)*255))
    G = int(round((G + m)*255))
    B = int(round((B + m)*255))

    return [R,G,B]

def convertYUVToRGBComponents(Y, U, V):
    R = int(round(Y + 1.4075 * (V - 128)))
    G = int(round(Y - 0.3455 * (U - 128) - (0.7169 * (V - 128))))
    B = int(round(Y + 1.7790 * (U - 128)))

    return [R,G,B]

def convertYCbCrToRGBComponents(Y, Cb, Cr):
    R = int(round((1.164 * (Y - 16)) + (0 * (Cb - 128)) + (1.596 * (Cr - 128))))
    G = int(round((1.164 * (Y - 16)) + (-.392 * (Cb - 128)) + (-.813 * (Cr - 128))))
    B = int(round((1.164 * (Y - 16)) + (2.017 * (Cb - 128)) + (0 * (Cr - 128))))

    return [R,G,B]

def convertYIQToRGBComponents(Y, I, Q):
    R = int(round((1 * Y) + (.956 * I) + (.621 * Q)))
    G = int(round((1 * Y) + (-.272 * I) + (-.647 * Q)))
    B = int(round((1 * Y) + (-1.105 * I) + (1.702 * Q)))

    return [R,G,B]

def convertXYZToRGBComponents(X, Y, Z):
    var_X = X / 100.0
    var_Y = Y / 100.0
    var_Z = Z / 100.0

    var_R = var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986
    var_G = var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415
    var_B = var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570

    if var_R > 0.0031308:
        var_R = 1.055 * ( var_R ** ( 1 / 2.4 ) ) - 0.055
    else:
        var_R = 12.92 * var_R

    if var_G > 0.0031308:
        var_G = 1.055 * ( var_G ** ( 1 / 2.4 ) ) - 0.055
    else:
        var_G = 12.92 * var_G

    if var_B > 0.0031308:
        var_B = 1.055 * ( var_B ** ( 1 / 2.4 ) ) - 0.055
    else:
        var_B = 12.92 * var_B

    R = int(round(var_R * 255.0))
    G = int(round(var_G * 255.0))
    B = int(round(var_B * 255.0))

    return [R,G,B]

# decides which color space conversion model to choose based on color space requested by user
def convertToRGBComponents(choice, component1, component2, component3):
    if choice == 1:
        return [int(round(component1)),int(round(component2)),int(round(component3))]
    elif choice == 2:
        return convertXYZToRGBComponents(component1, component2, component3)
    elif choice == 3:
        return convertLabToRGBComponents(component1, component2, component3)
    elif choice == 4:
        return convertYUVToRGBComponents(component1, component2, component3)
    elif choice == 5:
        return convertYCbCrToRGBComponents(component1, component2, component3)
    elif choice == 6:
        return convertYIQToRGBComponents(component1, component2, component3)
    elif choice == 7:
        return convertHSLToRGBComponents(component1, component2, component3)
    else:
        print "Please enter correct choice. Exiting code."
        sys.exit(0)

########################################################################################################################
#                                               Main Code                                                              #
########################################################################################################################
colorSpaceChoices = []
colorSpaceChoices.append("RGB")
colorSpaceChoices.append("XYZ")
colorSpaceChoices.append("Lab")
colorSpaceChoices.append("YUV")
colorSpaceChoices.append("YCbCr")
colorSpaceChoices.append("YIQ")
colorSpaceChoices.append("HSL")

print "Select the color space"
print "1. " + colorSpaceChoices[0]
print "2. " + colorSpaceChoices[1]
print "3. " + colorSpaceChoices[2]
print "4. " + colorSpaceChoices[3]
print "5. " + colorSpaceChoices[4]
print "6. " + colorSpaceChoices[5]
print "7. " + colorSpaceChoices[6]

choice = int(raw_input("Choice: "))

print "Enter values for first color (Cit) in range " + getComponentRange(choice)
c1r = float(raw_input("Enter first component : "))
c1g = float(raw_input("Enter second component : "))
c1b = float(raw_input("Enter third component : "))

print "Enter values for second color (Ci0) in range " + getComponentRange(choice)
c2r = float(raw_input("Enter first component : "))
c2g = float(raw_input("Enter second component : "))
c2b = float(raw_input("Enter third component : "))

print "Enter values for third color (CiT) in range " + getComponentRange(choice)
c3r = float(raw_input("Enter first component : "))
c3g = float(raw_input("Enter second component : "))
c3b = float(raw_input("Enter third component : "))

b = int(raw_input("Enter number of bits for color map ( b ): "))

# total mumner of intervals
intervalCount = 2**b

# coordinates from where the plotting of color space will be plotted om image
x = 200
y = 200

# height and width of the rectangle used to present the color
height = 50
width = 100

img = np.zeros(((intervalCount)*height + 500,1000,3), np.uint8)
img[:,0:1000] = (255,255,255)      # (B, G, R)

floatValuePrecision = 4
floatFormat =  "{:."+str(floatValuePrecision)+"f}"

cv2.putText(img,"Color Map for three colors ( "+str(c1r)+" , "+str(c1g)+" , "+str(c1b)+" ) -> ( "+str(c2r)+" , "+str(c2g)+" , "+str(c2b)+" ) -> ( "+str(c3r)+" , "+str(c3g)+" , "+str(c3b)+" )", (x,y), cv2.FONT_HERSHEY_COMPLEX, 0.37, 0)

x += 200
y += 100

f = open("ColorMapASCII-" +  colorSpaceChoices[choice - 1] + "-" + str(c1r) + "-" + str(c1g) + "-" + str(c1b) + "-" + str(c2r) + "-" + str(c2g) + "-" + str(c2b) + "-" + str(c3r) + "-" + str(c3g) + "-" + str(c3b) + ".txt", 'w')

for intervalIndex in range(0, (2**b)):

    t = intervalIndex % 2**(b-1)

    if intervalIndex/(2**(b-1)) == 0:
        component3 = c1b + (c2b -c1b) * (2 * t + 1)/(2**(b))
        component2 = c1g + (c2g -c1g) * (2 * t + 1)/(2**(b))
        component1 = c1r + (c2r -c1r) * (2 * t + 1)/(2**(b))
    elif intervalIndex/(2**(b-1)) == 1:
        component3 = (c2b + (c3b -c2b) * (2.0 * t + 1)/(2**(b)))
        component2 = (c2g + (c3g -c2g) * (2.0 * t + 1)/(2**(b)))
        component1 = (c2r + (c3r -c2r) * (2.0 * t + 1)/(2**(b)))

    transformedComponents = convertToRGBComponents(choice, component1, component2, component3)
    for i in range(0, 3):
        if transformedComponents[i] > 255:
            transformedComponents[i] = 255;
        if transformedComponents[i] < 0:
            transformedComponents[i] = 0;

    cv2.putText(img,floatFormat.format((-1.0+(2.0*intervalIndex/intervalCount))), (x - 75, y + intervalIndex * height), cv2.FONT_HERSHEY_COMPLEX, 0.37, 0)
    rect = cv2.rectangle(img, (x , y + intervalIndex * height), (x + width, y + height + intervalIndex * height),(transformedComponents[2], transformedComponents[1], transformedComponents[0]),-1)
    cv2.putText(img,"( "+str(getFormattedValue(component1,choice))+" , "+str(getFormattedValue(component2,choice))+" , "+str(getFormattedValue(component3,choice))+" )", (x + width + width/5, y + intervalIndex * height + height/2 + 5), cv2.FONT_HERSHEY_COMPLEX, 0.37, 0)

    fileContent = floatFormat.format((-1.0+(2.0*intervalIndex/intervalCount)))+":"+str(transformedComponents[0])+","+str(transformedComponents[1])+","+str(transformedComponents[2])
    f.write(fileContent)
    f.write('\n')

f.close()
#cv2.imshow("rect", img)
cv2.imwrite("ColorMap-" + colorSpaceChoices[choice - 1] + "-" + str(c1r) + "-" + str(c1g) + "-" + str(c1b) + "-" + str(c2r) + "-" + str(c2g) + "-" + str(c2b) + "-" + str(c3r) + "-" + str(c3g) + "-" + str(c3b) + ".jpg",img)

print "\n\n*******************Important******************"
print "Please check the output image ColorMap.jpg and ASCII text ColorMapASCII.txt for the given color map."

cv2.waitKey(0)
cv2.destroyAllWindows()
