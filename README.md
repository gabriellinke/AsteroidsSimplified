#Documentação auxiliar para os desenvolvedores

### MENSAGENS NA FILA DO BOTÃO:
O único evento é o de pressionamento do botão. É enviado um inteiro para identificar o evento.
 - Bits de identificação (b31, b30): 00
 - Payload (b29, b0): não tem
Acredito que esse evento poderia ser uma event flag.

### MENSAGENS NA FILA DO TOUCH:
O único evento é o de pressionamento do touch. É enviado um identificador e as coordenadas x,y do toque.
 - Bits de identificação (b31, b30): 10
 - Payload (b29, b0): 
  - (b29, b15) - 15 bits para identificar a coordenada x do local pressionado
  - (b14, b0) - 15 bits para identificar a coordenada y do local pressionado

### MENSAGENS NA FILA DE CONTROLE:
Os eventos poderão ser de dois tipos: SHOT ou MOVE. Os dois primeiros bits serão usados para a identificação do evento e os bits restantes para transmitir o payload de cada eventos

1. SHOT:
 - Bits de identificação (b31, b30): 00
 - Payload (b29, b0): não tem
 
2. MOVE:
 - Bits de identificação (b31, b30): 10
 - Payload (b29, b0): 
  - (b29, b15) - 15 bits para identificar a coordenada x do local pressionado
  - (b14, b0) - 15 bits para identificar a coordenada y do local pressionado
  
### MENSAGENS NA FILA DE GRÁFICOS:
Essa fila será utilizada para passar mensagens com informações de quais objetos existem na tela e em que posição eles estão.
 - Bits de identificação (b31 - b18)
  - ID (b31 - b19)
  - Tipo (b18 - b16)
 - Payload (b17 - b0)
  - Opção 1 - Posição:
   - Posição x (b17 - b9)
   - Posição y (b8 - b0)
  - Opção 2 - Rotação da nave em graus 
   - Rotação (b8 - b0)
  - Opção 3 - Pontuação do jogador
   - Pontuação (b17-b0)
 

0 -> Nave 
1 -> Projétil
2 -> Asteroide grande
3 -> Asteroide pequeno
4 -> Pontuação
5 -> Recorde de pontuação


Fazer um vetor de widgets/ids depois que criar os widgets.
struct widget {
	GX_WIDGET widget;
	int id;
} typedef CustomWidget

Quando a função de desenho for chamada, verificar se já existe o widget com esse ID.
Se não existir, cria o widget e coloca na lista de widgets.
Se existir, pega o widget com esse ID da lista e desenha. 