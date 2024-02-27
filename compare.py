from PIL import Image
from sys import os

def compare_tga(file1, file2):
    # Open the TGA files
    with Image.open(file1) as img1, Image.open(file2) as img2:
        # Ensure the images are the same size
        if img1.size != img2.size:
            return 0
            
        total_pixels = 0
        bad_pixels = 0

        # Compare each pixel
        width, height = img1.size
        for x in range(width):
            for y in range(height):
                total_pixels+=1
                if img1.getpixel((x, y)) != img2.getpixel((x, y)):
                    bad_pixels+=1


        return (float((float(total_pixels-bad_pixels)) / float(total_pixels)))


def main():
    name = 
    for root, dirs, files in os.walk(f"extracted_submissions/{name}/output"):
        num_files = 0
        score = 0.0
        for file in files:
            try:
                out = compare_tga(f"extracted_submissions/{name}/output/{file}", f"extracted_submissions/{name}/examples/EXAMPLE_{file}")
                score += out 
            except:
                pass
                

    score = round((float(score) / 12.0)*100, 2) # a perfect score is a 108
    print(f"Final Score: {score}")
    

main()