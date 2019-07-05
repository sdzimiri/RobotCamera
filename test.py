import argparse
from lucam import Lucam
import time

start_time = time.time()

parser = argparse.ArgumentParser('take Lucam picture')
parser.add_argument('--fname', type = str, help = 'file name of image to be saved')

if __name__ == '__main__':

    args = parser.parse_args()
    fname = args.fname
    

    lucam = Lucam(1)
    pixeldepth= lucam.GetTruePixelDepth()

    print('Pixel Depth', pixeldepth)
   
    camera = Lucam()
    image = camera.TakeSnapshot()
    camera.SaveImage(image, fname)

    tot_time = time.time() - start_time

    with open(str(time.time()) + '.txt', 'w') as f:
        f.write(str(tot_time))


    #print('took image!')







































































































































































