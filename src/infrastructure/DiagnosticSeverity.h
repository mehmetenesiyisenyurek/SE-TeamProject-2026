//
// Created by hseyi on 1.05.2026.
//

#ifndef SE_TEAMPROJECT_2026_DIAGNOSTICSEVERITY_H
#define SE_TEAMPROJECT_2026_DIAGNOSTICSEVERITY_H

//Bulguların önem seviyesini tanımlar arayüzde renk kodlaması olarak kullanılır
enum class DiagnosticSeverity {
    CRITICAL, //Derleme hatası sorunları (Kırmızı)
    WARNING, //Potansiyel sorunlar,kötü pratikler (Sarı)
    INFO //Bilgilendirme,stil önerileri (Mavi)
};


#endif //SE_TEAMPROJECT_2026_DIAGNOSTICSEVERITY_H
