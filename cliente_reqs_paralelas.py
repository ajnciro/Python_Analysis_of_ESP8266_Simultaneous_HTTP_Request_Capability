"""AIOHTTP é um framework de cliente/servidor assíncrono para
requisições HTTP. É útil para para envio de múltimas requisições
paralelas. Neste caso específico, em vez de mascarar o IP do cliente
para vários IPs distintos, as requisições são feitas para os
diversos diretórios encontrados no servidor através de portas
de origem diferentes, de modo virtualmente simultâneo em tempo
de execução. Para este caso específico, as requições das páginas
de /0 até /99 foram feitas pseudoaleatoriamente, com número de requisições
simultâneas variando de 1 até 100, repetidas 5 vezes para cada, a
fim de se obter uma distribuição estatística"""

import asyncio
import aiohttp
import nest_asyncio
import time
import pandas as pd
from random import randrange as rr

nest_asyncio.apply()                            #Uma função para evitar erros de execuções em andamento

ur = "http://192.168.100.70"                    #O edereço do servidor. É necessário que o URL esteja completo.
ur+="/{}"

dataframe = pd.DataFrame([], columns=["sim_reqs", "taxa_sucesso", "tempo_decorr"])
"""Instanciamento do DataFrame para o armazenamento dos dados"""

for requisicoes_simultaneas in range(1,100):    #Requisições simultâneas
    
    repeticoes = 0
    while(repeticoes < 5):                      #Número de repetições para as requisições
    
        is_sucesso = []
        
        urls = [ur.format(rr(0,100,1)) for _ in range(requisicoes_simultaneas)]
        
        async def get(url):
            """Funcões abaixo estabelecem as requisições HTTP"""
            try:
                async with aiohttp.ClientSession() as session:
                    async with session.get(url=url) as response:
                        await response.read()
                        is_sucesso.append(1)    #Caso a conexão seja bem-sucedida e haja uma reposta OK, será anexado um marcado a uma lista
                        print("200: {}".format(url[21:]))
            except Exception:
                is_sucesso.append(0)            #Caso malsucedida, anexa-se um valor 0.
                print("Erro: {}.".format(url[21:]))
        
        
        async def main(urls, amount):
            await asyncio.gather(*[get(url) for url in urls])
            print("Finalizado - sim_req: {}.{}".format(requisicoes_simultaneas,repeticoes))
        
        ini = time.time()                       #para fins de monitoramento do tempo gasto para gerar as requisições/respostas
        asyncio.run(main(urls, len(urls)))
        fim = time.time()
        
        lista =pd.DataFrame([[requisicoes_simultaneas,sum(is_sucesso)/len(is_sucesso),fim - ini]],
                            columns=["sim_reqs", "taxa_sucesso", "tempo_decorr"])
        dataframe = dataframe.append(lista,ignore_index=True)
        
        repeticoes+=1
    
    dataframe.to_pickle("requests_dataframe.pkl")   #Guarda arquivo contendo o DataFrame com os dados recolhidos na pasta de onde este script está executando.

#display(dataframe)         #Caso se deseje exibir no console o dataframe formatado, recomendo utilizar a função <<display>> em vez de <<print>>
