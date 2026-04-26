//
// Created by hseyi on 26.04.2026.
//

#pragma once
#include <string>
#include <chrono> //DateTime için kullanacağımız kütüphane

#ifndef SE_TEAMPROJECT_2026_SOURCEFILE_H
#define SE_TEAMPROJECT_2026_SOURCEFILE_H


class SourceFile {
private:
    std::string filePath;
    std::string fileName;
    std::string content;
    long fileSize;
    std::chrono::system_clock::time_point loadDate;

public:
    //Constuctor (loadDate'i otomatik atar)
    //const ve & direk adresteki değeri alır kopyalama yapmaz program daha hızlı çalışır
    SourceFile(const std::string& filePath,const std::string& fileName,const std::string& content,long fileSize);

    [[nodiscard]] std::string getContent() const; //Dosya içeriğini döndürür
    [[nodiscard]] std::string getFileName() const; //Dosya adını döndürür
    [[nodiscard]] std::string getFilePath() const; //Tam yolu döndürür
    [[nodiscard]] long getFileSize() const;//Boyutu döndürür
    [[nodiscard]] bool isEmpty() const; //İçerik boşsa true döndürür

};


#endif //SE_TEAMPROJECT_2026_SOURCEFILE_H
