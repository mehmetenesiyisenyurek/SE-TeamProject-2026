# Code Sentinel - Modern WinAPI Arayuz

Bu surum Qt, Qt Designer veya ekstra kutuphane kullanmaz. CLion + MinGW ile Windows'ta direkt calisir.

## Calistirma

1. Proje klasorunu CLion ile ac.
2. CMake reload yap.
3. Sag ustten `CodeSentinelDesktop` target'ini sec.
4. Run'a bas.

## Bu surumde duzeltilenler

- Tam ekran yapinca butonlarin ve dosya yolu alaninin birbirine girmesi duzeltildi.
- Ust bolum iki satira ayrildi: butonlar/filtreler ustte, dosya yolu altta.
- Minimum pencere boyutu eklendi.
- Kod ve rapor alanlari `EDIT` kontrolu olarak kullaniliyor; repaint/ust uste yazi problemi azaltilmistir.
- Critical / Warning / Info checkbox filtreleri korunmustur.
- HTML / JSON / TXT rapor ciktilari aktif filtreye gore kaydedilir.

## Not

Bu uygulama WinAPI kullandigi icin sadece Windows'ta `CodeSentinelDesktop` target'i olusur. Konsol surumu icin `CodeSentinelCLI` target'i kullanilabilir.
