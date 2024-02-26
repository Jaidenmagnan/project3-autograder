#include "Image.h"


Image::Image(ifstream* tga){
    //initializes size of instance variables
    idLength = new char(1);
    colorMapType = new char(1);
    dataTypeCode= new char(1);
    colorMapOrigin= new short(2);
    colorMapLength= new short(2);
    colorMapDepth= new char(1);
    xOrigin = new short(2);
    yOrigin= new short(2);
    width= new short(2);
    height= new short(2);
    bitsPerPixel= new char(1);
    imageDescriptor= new char(1);
    //read header data into instance variables
    tga->read((char*)this-> idLength, 1);
    tga->read(this-> colorMapType, 1);
    tga->read(this-> dataTypeCode, 1);
    tga->read((char*)this-> colorMapOrigin, 2);
    tga->read((char*)this-> colorMapLength, 2);
    tga->read(this-> colorMapDepth, 1);
    tga->read((char*)this-> xOrigin, 2);
    tga->read((char*)this-> yOrigin, 2);
    tga->read((char*)this-> width, 2);
    tga->read((char*)this-> height, 2);
    tga->read(this-> bitsPerPixel, 1);
    tga->read(this-> imageDescriptor, 1);
    size = int(*width)* int(*height)*3;
    pixelData = new unsigned char[size];
    tga->read((char*)this->pixelData, size); //reads until end of file
    tga->seekg(0); // reset position to beginning of file
    tga->read((char*)this->headerData, 18);

}
//default constructor
Image::Image(){}

//returns array of pixel data
unsigned char* Image::getPixelData(){
    return this->pixelData;
}

//set pixel data to new array of pixels
void Image::setPixelData(unsigned char* newPixelData){
    for(int i=0; i<this->size;i++){
        this->pixelData[i]=newPixelData[i];
    }
}

void Image::resizePixelData(unsigned char* newPixelData, int size){
    delete[]pixelData;
    pixelData= new unsigned char[size];
    for(int i=0; i<size; i++){
        pixelData[i]=newPixelData[i];
    }
}
//returns char array with contents of header
unsigned char* Image::getHeaderData(){
    return this->headerData;
}
//assign correct width and height in header data
void Image::resetHeaderData(){
    unsigned char widthBytes[2];
    unsigned char heightBytes[2];
    memcpy(widthBytes, width, 2);
    memcpy(heightBytes, height, 2);
    this->headerData[12]=widthBytes[0];
    this->headerData[13]=widthBytes[1];
    this->headerData[14]=heightBytes[0];
    this->headerData[15]=heightBytes[1];
}
//returns width*height*3
int Image::getSize(){
    size = int(*width)* int(*height)*3;
    return this->size;
}

int Image::getWidth(){
    return (int)*width;
}
int Image::getHeight(){
    return (int)*height;
}

void Image::setWidth(short newWidth){
    //set width = to new width address
    delete width;
    this->width= new short(newWidth);
}
void Image::setHeight(short newHeight){
    //set height = to new height address
    delete height;
    this->height= new short(newHeight);
}

Image Image::multiply(Image& bottomLayer){
    unsigned char* tempPixels= new unsigned char[this->size];
    float newPixel;
    float topPixel;
    float bottomPixel;
    for(int i=0; i<this->size; i++){
        topPixel = ((float)this->pixelData[i])/255;
        bottomPixel = ((float)bottomLayer.getPixelData()[i])/255;
        newPixel= (topPixel*bottomPixel)*255;
        newPixel+=0.5; //rounding
        tempPixels[i] = (unsigned char) newPixel;
    }
    Image newImage=*this;
    newImage.setPixelData(tempPixels);
    delete[] tempPixels;
    return newImage;
}

Image Image::subtract(Image& bottomLayer){
    //bottom layer - top layer
    unsigned char* tempPixels= new unsigned char[this->size];
    float newPixel;
    float topPixel;
    float bottomPixel;
    for(int i=0; i<this->size; i++){
        topPixel = (float)this->pixelData[i];
        bottomPixel = (float)bottomLayer.getPixelData()[i];
        newPixel= (bottomPixel-topPixel);
        if(newPixel>255){
            newPixel=255;
        }
        else if(newPixel<0){
            newPixel=0;
        }
        tempPixels[i] = (unsigned char) newPixel;
    }
    Image newImage=*this;
    newImage.setPixelData(tempPixels);
    delete[] tempPixels;
    return newImage;
}
Image Image::screen(Image& bottomLayer){
    unsigned char* tempPixels= new unsigned char[this->size];
    float newPixel;
    float topPixel;
    float bottomPixel;
    for(int i=0; i<this->size; i++){
        topPixel = ((float)this->pixelData[i])/255;
        bottomPixel = ((float)bottomLayer.getPixelData()[i])/255;
        newPixel= (1-(1-topPixel)*(1-bottomPixel))*255;
        newPixel+=0.5; //rounding
        tempPixels[i] = (unsigned char) newPixel;
    }
    Image newImage=*this;
    newImage.setPixelData(tempPixels);
    delete[] tempPixels;
    return newImage;
}
Image Image::overlay(Image& bottomLayer){
    unsigned char* tempPixels= new unsigned char[this->size];
    float newPixel;
    float topPixel;
    float bottomPixel;
    for(int i=0; i<this->size; i++){
        topPixel = ((float)this->pixelData[i])/255;
        bottomPixel = ((float)bottomLayer.getPixelData()[i])/255;
        if(bottomPixel*255<=(255/2)){
            newPixel= 2*topPixel*bottomPixel*255;
            if(newPixel>255){
                newPixel=255;
            }
        }
        else{
            newPixel= (1-(2*(1-topPixel)*(1-bottomPixel)))*255;
            if(newPixel>255){
                newPixel=255;
            }
            else if(newPixel<0){
                newPixel=0;
            }
        }
        newPixel+=0.5; //rounding
        tempPixels[i] = (unsigned char) newPixel;
    }
    Image newImage=*this;
    newImage.setPixelData(tempPixels);
    delete[] tempPixels;
    return newImage;
}
Image Image::addChannel(string channel, float value){
    int channelNumber;
    if(channel=="red"){
        channelNumber = 2;
    }
    else if(channel=="green"){
        channelNumber= 1;
    }
    else if(channel=="blue"){
        channelNumber=0;
    }
    unsigned char* tempPixels= new unsigned char[this->size];
    float oldPixel;
    float newPixel;
    //copy pixelData into tempPixels
    for(int i=0; i<this->size;i++){
        tempPixels[i]=this->pixelData[i];
    }
    //only affects specified channel
    for(int i=channelNumber; i<this->size; i+=3){
        oldPixel = float(tempPixels[i]);
        newPixel= oldPixel+value;
        if(newPixel>255){
            newPixel=255;
        }
        else if(newPixel<0){
            newPixel=0;
        }
        tempPixels[i]= (unsigned char)newPixel;
    }
    Image newImage=*this;
    newImage.setPixelData(tempPixels);
    delete[]tempPixels;
    return newImage;
}
Image Image::scaleChannel(string channel, int value){
    int channelNumber;
    if(channel=="red"){
        channelNumber = 2;
    }
    else if(channel=="green"){
        channelNumber= 1;
    }
    else if(channel=="blue"){
        channelNumber=0;
    }
    unsigned char* tempPixels= new unsigned char[this->size];
    float oldPixel;
    float newPixel;
    //copy pixelData into tempPixels
    for(int i=0; i<this->size;i++){
        tempPixels[i]=this->pixelData[i];
    }
    //only affects specified channel
    for(int i=channelNumber; i<this->size; i+=3){
        oldPixel = float(tempPixels[i]);
        newPixel= oldPixel*value;
        if(newPixel>255){
            newPixel=255;
        }
        else if(newPixel<0){
            newPixel=0;
        }
        tempPixels[i]= (unsigned char)newPixel;
    }
    Image newImage=*this;
    newImage.setPixelData(tempPixels);
    delete[]tempPixels;
    return newImage;
}
Image Image::rotate(){
    unsigned char* tempPixels= new unsigned char[this->size];
    //traverse backwards
    int count=0;
    for(int i=this->size-1; i>=0; i-=3){
        tempPixels[count]=this->pixelData[i-2];
        count++;
        tempPixels[count]=this->pixelData[i-1];
        count++;
        tempPixels[count]=this->pixelData[i];
        count++;
    }
    Image newImage=*this;
    newImage.setPixelData(tempPixels);
    delete[]tempPixels;
    return newImage;
}
void Image::deleteVars(){
    delete this->idLength;
    delete this->colorMapType;
    delete this->dataTypeCode;
    delete this->colorMapOrigin;
    delete this->colorMapLength;
    delete this->colorMapDepth;
    delete this->xOrigin;
    delete this->yOrigin;
    delete this->width;
    delete this->height;
    delete this->bitsPerPixel;
    delete this->imageDescriptor;
    delete[] this->pixelData;
}

//BIG-3

Image::Image(Image& other){
    this->idLength=new char(*other.idLength);
    this->colorMapType=new char(*other.colorMapType);
    this->dataTypeCode= new char(*other.dataTypeCode);
    this->colorMapOrigin=new short(*other.colorMapOrigin);
    this->colorMapLength= new short(*other.colorMapLength);
    this->colorMapDepth= new char(*other.colorMapDepth);
    this->xOrigin=new short(*other.xOrigin);
    this->yOrigin= new short(*other.yOrigin);
    this->width= new short(*other.width);
    this->height=new short(*other.height);
    this->bitsPerPixel=new char(*other.bitsPerPixel);
    this->imageDescriptor=new char(*other.imageDescriptor);
    this->size=other.size;
    for(int i= 0; i<18; i++){
        this->headerData[i]=other.headerData[i];
    }
    this->pixelData = new unsigned char [(int)*width*(int)*height*3];
    for(int i= 0; i<(int)*width*(int)*height*3; i++){
        this->pixelData[i]=other.pixelData[i];
    }
}

Image& Image:: operator=(Image& other){
    this->deleteVars();
    this->idLength=new char(*other.idLength);
    this->colorMapType=new char(*other.colorMapType);
    this->dataTypeCode= new char(*other.dataTypeCode);
    this->colorMapOrigin=new short(*other.colorMapOrigin);
    this->colorMapLength= new short(*other.colorMapLength);
    this->colorMapDepth= new char(*other.colorMapDepth);
    this->xOrigin=new short(*other.xOrigin);
    this->yOrigin= new short(*other.yOrigin);
    this->width= new short(*other.width);
    this->height=new short(*other.height);
    this->bitsPerPixel=new char(*other.bitsPerPixel);
    this->imageDescriptor=new char(*other.imageDescriptor);
    this->size=other.size;
    for(int i= 0; i<18; i++){
        this->headerData[i]=other.headerData[i];
    }
    this->pixelData = new unsigned char [(int)*width*(int)*height*3];
    for(int i= 0; i<(int)*width*(int)*height*3; i++){
        this->pixelData[i]=other.pixelData[i];
    }
    return *this;
}
Image::~Image(){
    delete this->idLength;
    delete this->colorMapType;
    delete this->dataTypeCode;
    delete this->colorMapOrigin;
    delete this->colorMapLength;
    delete this->colorMapDepth;
    delete this->xOrigin;
    delete this->yOrigin;
    delete this->width;
    delete this->height;
    delete this->bitsPerPixel;
    delete this->imageDescriptor;
    delete[] pixelData;
}

