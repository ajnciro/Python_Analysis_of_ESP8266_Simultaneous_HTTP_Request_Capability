//Para converter imagens para base64, utilizar, por exemplo, um serviço como https://www.base64-image.de/

#ifndef imagenzinha_h
#define imagenzinha_h

String ImgBase64 = "'data:image/jpeg;base64,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'";

#endif