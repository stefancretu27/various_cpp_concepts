CFLAGS = -std=c++17 -Wall -g
CC = g++
SRCS = TemplateMethods/TemplateMethods.cpp TemplateMethods/TemplateMethodsImpl.cpp TemplateMethods/TemplateMethodsDataTypes.cpp main.cpp
SRCS +=	TemplateClasses/TemplateClassFullSpecialization.cpp TemplateClasses/TemplateClassPartialSpecialization.cpp TemplateClasses/TemplateClassInstances.cpp

all:templates 

templates: 
	$(CC) $(CFLAGS) $(SRCS) -o templates.bin

clean:
	rm *.bin
