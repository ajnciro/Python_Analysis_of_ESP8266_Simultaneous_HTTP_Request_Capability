# 1 – Objetivo 

Desenvolver um servidor web multithread  sobre protocolo HTTP 1.0, capaz de atender a múltiplas requisições simultâneas de serviços em paralelo. 

# 2 – Material Utilizado 

Módulo WiFi com controlador ESP8266 integrado a uma interface serial e regulador de 3.3V.

# 3 – Fundamentos Teóricos 

### 1. – O Protocolo HTTP 1.0 

O HTTP 1.0  ou HTTP/1.0 (Hypertext Transfer Protocol) é um protocolo da camada de aplicação descrito na RFC 1945 [1], que trata de como as requisições de páginas  Web são feitas por cliente a servidores, bem como a estrutura das mensagens trocadas entre eles. 

O protocolo é, em si, stateless (não guarda o estado da ***conversa***), exige métodos atribuídos para a troca de mensagens – requisições, confirmações, negações, erros etc. –, e é referenciado, formatado e endereçado por meio de URIs: URLs, ou URNs.  

Entre os principais componentes do protocolo estão as requisições e as respostas. É sempre o agente de usuário Cliente que faz as requisições de conexão ou arquivos, e sempre o agente de usuário Servidor que responde ao cliente as solicitações. ![](Aspose.Words.33a3665f-147c-4636-b477-6d243817648b.004.png)

Uma mensagem de requisição sempre inclui na primeira linha o método aplicado ao recurso necessário, o identificador do recurso e a versão do protocolo utilizado, e esse método pode ser GET (para recuperar alguma informação), POST (para o envio de parâmetros dentro do corpo da requisição) ou HEAD (para obtenção de metainformação do servidor sem a necessidade de receber a entidade completa relacionada à página solicitada). 

Uma mensagem de resposta enviada após uma requisição sempre inclui primeiro uma linha 

de status com a versão do protocolo e um código de status, o qual pode ir no formato 1xx (Informativo, não utilizado em HTTP 1.0, mas em 1.1), 2xx (Sucesso), 3xx (Redirecionamento), 4xx (Erro no cliente) e 5xx  (Erro no servidor). 

Entre as principais diferenças entre o HTTP/1.0 e o HTTP/1.1 está a existência de conexões persistentes presentes no 1.1. Isto é dito, pois, como se verá a seguir, na depuração das mensagens trocadas entre cliente e o servidor implementado no projeto, aparecem respostas do tipo HTTP 1.1, porém como o cliente foi implementado de uma maneira que faz múltiplas requisições simultâneas sem necessidade de persistência, na prática se torna o servidor idêntico ao que se fosse construído sobre HTTP/1.0. 

### 2. – O ESP8266 

O ESP8266 é um microcontrolador com capacidade de comunicação WiFi para conexões TCP/IP entre dispositivos sem fio. Entre as diversas formas de implementação do CI, existe o módulo D1 mini, como o seguinte: 

![](/img/Aspose.Words.33a3665f-147c-4636-b477-6d243817648b.005.jpeg)

o qual inclui extensões da pinagem, regulador de tensão em 3V3 e comunicação serial via micro USB. 

Originalmente concebido para ser programado via comando AT, atualmente conta com SDKs modernos, e pode ser utilizado com uma IDE para C++, por exemplo. Ao utilizar uma IDE para C++, como  a  Arduino  IDE,  é  necessário  a  inclusão  de  bibliotecas  de  apoio  ao  WiFi  e  arquitetura cliente/servidor como a ***WiFiClient.h*** e a ***ESP8266WebServer***. Detalhes sobre as bibliotecas de suporte em [2]. 

# 4 – Procedimento Experimental 

Um servidor web apto a responder requisições HTTP foi implementado sobre um módulo ESP8266 ligado a uma rede local. O código referente à implementação pode ser encontrado no arquivo ***ServidorHTTPEsp8266.txt***, o qual está devidamente comentado para melhor compreensão. 

O servidor armazena uma pequena página web contento um título, um corpo textual, um arquivo de imagem, e um campo para envio das leituras feitas através do módulo. Como segue a imagem: 

![](/img/Aspose.Words.33a3665f-147c-4636-b477-6d243817648b.006.png)

É enviado ao cliente, quando este realiza a requisição, um texto contendo a página requerida, uma imagem, e um texto contendo uma leitura do sensor. À leitura do sensor não foi atribuída uma grandeza específica, ficando ao gosto do usuário definir qual tipo de dado desejar enviar, podendo ser dados de temperatura, tensão elétrica, nível de intensidade sonora, presença, detecção química etc., bastando apenas a ligação do sensor adequando ao GPIO correto. 

O conjunto de páginas geradas percorre um alcance desde a raiz ***/***, até páginas de ***/0*** a ***/99***, bem como uma mensagem de página não encontrada. Isso também está especificado no código fonte, e essa quantidade de páginas foi construída de modo a permitir que um cliente faça requisição de vários objetos simultaneamente, por meio de diversas portas, a fim de se testar a capacidade de atendimento do servidor. 

É importante notar que há a importação de uma biblioteca denominada ***imagenzinha.h***, a qual fornece a codificação de um arquivo de imagem para a texto em base64, apenas com o objetivo de não poluir o corpo do código fonte. 

Após o servidor estar em funcionamento, as requisições podem ser feitas por meio de um browser comum, onde se visualizará o que foi representado acima, mas também pode ser feito por meio de um cliente implementado automaticamente. 

Assim sendo, um cliente que faz múltiplas requisições paralelas foi utilizado, e o script para implementação do mesmo está codificado no arquivo ***cliente\_reqs\_paralelas.py*** [3]. Este script realiza uma bateria de mesma quantidade de requisições paralelas, 5 vezes para uma determinada quantidade, e varia o número de requisições de 1 até 100 simultâneas. Ele recolhe a resposta enviada (ou não enviada) pelo servidor, e salva um registro do sucesso das conexões em um arquivo de DataFrame com o nome de ***requests\_dataframe.pkl***[4]. 

Julgou-se necessário a exportação de um dataframe para facilitar a análise das requisições, o que será apresentado a seguir. 

Também, simultaneamente ao registro dos sucessos de conexão feitos pelo script do cliente, um sniffing pelo WireShark foi colocado para atuar com o objetivo de registar as conversas entre os agentes de usuário. As conversas HTTP foram guardadas em valores separados por vírgula em um arquivo chamado ***ws\_pcap.csv***, também com o objetivo de análise do tráfego durante as requisições respostas entre os hosts. 

A análise foi realizada utilizando um conjunto de soluções de console interativo de  Python denominada  Jupyter. O texto com os códigos está no arquivo ***analise\_pcap.ipynb***. 

# 5 – Resultados 

É  possível  observar  no  texto  descrito  no  arquivo ***analise\_pcap.ipynb*** que os registros gerados pelas requisições do cliente são da forma: 

![](/img/Aspose.Words.33a3665f-147c-4636-b477-6d243817648b.007.jpeg)

Em que sim\_reqs é a coluna com o número de requisições simultâneas feitas ao servidor, **taxa\_sucesso** é a coluna de conexões bem-sucedidas: 

![](/img/Aspose.Words.33a3665f-147c-4636-b477-6d243817648b.008.png)

𝑡𝑎𝑥𝑎 𝑑𝑒 𝑠𝑢𝑐𝑒𝑠𝑠𝑜 = $\frac{Respostas}{RequisiçõesEnviadas}$


ou  seja,  a  fração  de  respostas  recebidas  em  todas  as  requisições  enviadas  simultaneamente,  e **tempo\_decorr** é o tempo decorrido desde que a primeira requisição foi enviada até a última resposta não ser recebida. Isso de uma variedade de 1 até 100 requisições simultâneas, 5 vezes para cada sim\_req. 

O dataframe agrupado por classe de requisições simultâneas: 

![](/img/Aspose.Words.33a3665f-147c-4636-b477-6d243817648b.009.jpeg)

e com a média aplicada sobre as colunas, demonstra que a taxa de sucesso cai tanto quanto o número de requisições simultâneas e paralelas aumenta. De fato, a partir da análise contida no Notebook, é possível notar que a taxa de sucesso é máxima até 25 pedidos simultâneos. A partir de então, ou o servidor deixa de receber os pedidos, ou, tendo recebido o pedido, deixa de enviar as respostas. Além disso, o próprio tempo necessário para totalizar as conexões aumenta. 

Uma  grandeza  talvez  interessante  de  se  observar  seria  o  número  de  requisições  bem- sucedidas feitas unidade de tempo. Do dataframe: 

![](/img/Aspose.Words.33a3665f-147c-4636-b477-6d243817648b.010.jpeg)

é possível notar que existe um máximo de requisições bem-sucedidas por unidade de tempo que está por volta de 5 requisições simultâneas, em que o tempo de resposta é mínimo, geral algo torno de 6 a 7 requisições bem-sucedidas por segundo, como no gráfico: 

![](/img/Aspose.Words.33a3665f-147c-4636-b477-6d243817648b.011.jpeg)

E, a partir de 25 paralelas, o tempo para todas as respostas permanece praticamente o mesmo: 

![](/img/Aspose.Words.33a3665f-147c-4636-b477-6d243817648b.012.jpeg)

E a taxa de sucesso começa cair: 

![](/img/Aspose.Words.33a3665f-147c-4636-b477-6d243817648b.013.jpeg)

Uma análise similar também pode ser encontrada no arquivo anexo quando se observa o registro das conversas consecutivas captadas pelo  WireShark, e que, de fato, inclusive aumenta o tempo entre um pedido e uma resposta correspondente: 

![](/img/Aspose.Words.33a3665f-147c-4636-b477-6d243817648b.014.jpeg)

A capacidade montagem gráfica da mensagem enviada pelo servidor também foi testada. Quando se solicita uma página existente: 

![](/img/Aspose.Words.33a3665f-147c-4636-b477-6d243817648b.015.jpeg)

É exibida dessa maneira, e uma mensagem de controle é enviada por comunicação serial. Assim é com qualquer página desde a raiz até ***/99***: 

![](/img/Aspose.Words.33a3665f-147c-4636-b477-6d243817648b.016.jpeg)

Quando uma página solicitada não foi programada para ser exibida, o seguinte é capturado: 

![](/img/Aspose.Words.33a3665f-147c-4636-b477-6d243817648b.017.jpeg)

Completando os recursos mínimos de servidor HTTP. 

# 6 – Conclusões 

O servidor implementado em ESP8266 foi capaz de atentar a várias requisições simultâneas e paralelas de pedidos HTTP/1.0. De fato, sua capacidade gira em torno de 25 requisições simultâneas, chegando a um máximo de até 7 conexões bem-sucedidas por segundo 

Após 25 pedidos simultâneos, a capacidade de atender se prejudica, e um montante de mensagens é perdido pela rede. 

Também, as mensagens enviadas pelo servidor são perfeitamente capazes de serem montadas por um navegador comum, bem como todas as páginas programadas.  

Se for considerada a capacidade de processamento do hardware e tipo de página web hospedada, são número até satisfatórios para aplicações simples. 

# 7 – Referências: 

1. HTTP/1.0: < https://tools.ietf.org/html/rfc1945 > 
1. ESP8266: < https://github.com/esp8266/ > 
1. AIOHTTP: < https://docs.aiohttp.org/en/stable/ > 
1. A Biblioteca Pandas: < https://pandas.pydata.org/ > 
