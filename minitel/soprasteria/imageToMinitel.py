import sys
import os
from PIL import Image

#test arg
if len(sys.argv) != 2:
    sys.exit("Veuillez preciser un nom de fichier (PNG, JPEG, etc)")

#essai ouverture Image
try:
    im = Image.open(sys.argv[1])
    pix = im.load()
except:
    sys.exit("impossible d'ouvrir le fichier image")

#largeur et hauteur en pixel
width, height = im.size

#nom de la variable dans le croquis
nom = os.path.splitext(sys.argv[1])[0]

#verification
if (width/2) > 40:
    sys.exit("Erreur : l'image doit acroi 80 pixel de large max")
if height/3 > 24:
    sys.exit("Erreur : l'image doit avoir 72 pixels de haut max")
if width %2:
    sys.exit("Erreur : l'image doit avoir une largeur multiple de 2")
if height %3:
    sys.exit("Erreur : l'image doit avoir une hauteur multiple de 3")

print("// Image: " + nom + "(" + str(width) + "x" + str(height) + ")")

x= 0
y= 0

print("#define L_" + nom + "     " + str(width/2))
print("#define H_" + nom + "     " + str(height/3))

print("\nPROGMEM const byte " + nom + "[]={")

index=3
while y < height:
    print("  "),
    x = 0
    while x < width:
        val = 32
        val += 1 if (pix[x,y][index] > 0) else 0;
        val += 2 if pix[x+1,y][index] > 0 else 0;
        val += 4 if pix[x,y+1][index] > 0 else 0;
        val += 8 if pix[x+1,y+1][index] > 0 else 0;
        val += 16 if pix[x,y+2][index] > 0 else 0;
        val += 32 if pix[x+1,y+2][index] > 0 else 0;
        if y> (height-4) and x > (width-3):
            print(val),
        else:
            print(str(val) + ","),
        x += 2;
    print("")
    y += 3;
print("};")