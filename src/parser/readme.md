# ASTNodeType

ASTNodeType ast ağaçlarındaki düğümlerin hangi tip kod olduğunu tutar. Örneğin kod analiz edilirken int main(){...}
gördüğünde buna bir donksiyon diyecektir.

# ASTNode

AST kaynak kodu yapısal temsilini tutan ağaçtır. *ASTNode* ise bu ağacın tek bir düğümüdür.

# Parser

Lexer'ın ürettiği tokenleri ağaca dönüştürür. Parser eğer hata görürse durmaz bir sonraki ; veya } ifadesine kadar olan
tokenları atlar. Bu sayede tek bir seferde birdenf falza hatayı yakalayabilir.

