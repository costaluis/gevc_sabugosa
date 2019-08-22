import cv2
import numpy as np

def birds_eye(frame):

  #Obtem número de linhas e colunas do frame
  rows = frame.shape[0]
  cols = frame.shape[1]

  #Definição dos pontos
  pt1 = np.float32([[0,rows],[int(0.3*cols),int(0.3*rows)],[int(0.7*cols),int(0.3*rows)],[cols,rows]])
  pt2 = np.float32([[0,480],[0,0],[640,0],[640,480]])

  #Criação da Matriz de Tranformação
  M = cv2.getPerspectiveTransform(pt1,pt2)

  #Aplicação da Transformação
  dst = cv2.warpPerspective(frame,M,(640,480))

  return dst

def binary_img(img_org):

  #Aplicação de Blur na img original
  blur = cv2.GaussianBlur(img_org,(5,5),0)

  #Tranformação Img RGB -> HSV
  hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV) 

  #Valores base para binarizar img
  low_array = np.array([16,68,149])
  high_array = np.array([223,190,200])

  #Tranformação em imagem binária
  mask = cv2.inRange(hsv, low_array, high_array)

  #Aplicação de Filtros na imagem binária
  kernel = np.ones((5,5),np.uint8)
  mask = cv2.erode(mask,kernel,iterations = 1)
  mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)
  mask = cv2.dilate(mask,kernel,iterations = 1)
  mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)

  return mask

#Leitura do vídeo
video = cv2.VideoCapture("road6.mp4")

#Criação das Janelas
cv2.namedWindow('video',cv2.WINDOW_NORMAL)
cv2.namedWindow('IMG_BINARIA',cv2.WINDOW_NORMAL)
cv2.namedWindow('Bird eyes',cv2.WINDOW_NORMAL)

def histogram(bin_img):

  #Cria janela do Histograma
  cv2.namedWindow("Histogram",cv2.WINDOW_NORMAL)

  #Obtem o número de linhas e colunas da imagem
  rows = bin_img.shape[0]
  cols = bin_img.shape[1]

  #Cria uma matriz zerada para a imagem do histograma e 
  #seta todos os pixels em 255 (cor branca)
  hist = np.zeros([rows,cols,3],dtype=np.uint8)
  hist[:] = 255

  #Cria matriz nula com o número de colunas da imagem
  count = np.zeros(cols)

  #Define o limite superior e inferior no qual a imagem será analisada
  row_init = int(rows/2)
  row_fin = rows

  #Cria variáveis que serão utilizadas na média ponderada
  x_medio = 0
  soma_alt = 0

  #Inicio do loop percorrendo a imagem binária
  for col in range(int(0.1*cols), int(0.9*cols)):

    #Aloca o número de pontos brancos em determinada coluna na posição do vetor correspondente
    count[col] = int(np.sum(bin_img[row_init:row_fin,col])/255)

    #Desenha o um circulo na altura correspondente ao número de pixels
    cv2.circle(hist, (col,int(rows-count[col])),1,(255,0,0),3,8,0)

    #Recebe a coluna com maior número de pontos brancos
    pt_max = np.argmax(count)

    #Percorre o vetor nas proximidades da coluna com mais pontos
    for i in range(pt_max-30, pt_max+30):
      x_medio += count[col]*col
      soma_alt += count[col]

    #Imprime o histograma
    cv2.imshow("Histogram", hist)


  #Realiza a média ponderada e retorna a posição calculada, ou -1 caso não exista nenhum ponto branco.
  if soma_alt == 0:
    return [-1,-1]
  else:
    x_medio /= soma_alt
    return [int(x_medio),rows]

  
def sliding_window(pos,bird_img,bin_img):

  cv2.namedWindow("Sliding Window",cv2.WINDOW_NORMAL)
  count = np.zeros(bird_img.shape[0])

  slide_img = bird_img.copy()

  for i in range(1,15):
    pt_sup_esq = [pos[0]-50,pos[1]-15*i]
    pt_inf_dir = [pos[0]+50,pos[1]+15-15*(i-1)]
    num = 0
    dem = 0
    for j in range(pt_sup_esq[0],pt_inf_dir[0]):
      if j >= bird_img.shape[0]:
        continue
      count[j] = int(np.sum(bin_img[pt_sup_esq[1]:pt_inf_dir[1],j])/255)
      num += count[j]*j
      dem += count[j]
    if dem != 0:
      media = int(num / dem)
      pt_sup_esq[0] = media-50
      pt_inf_dir[0] = media+50
      cv2.rectangle(slide_img,tuple(pt_sup_esq),tuple(pt_inf_dir),(255,0,100),3)

  cv2.imshow("Sliding Window",slide_img)

  


while True:

  #Leitura de um frame
  ret,frame = video.read()

  #Verifica se o vídeo já acabou, caso sim, retorna ao o mesmo ao início
  if ret is not True:
    video.set(cv2.CAP_PROP_POS_FRAMES, 0)
    ret,frame = video.read()

  dst = birds_eye(frame)

  mask = binary_img(dst)

  valor = histogram(mask)

  sliding_window(valor,dst,mask)

  print(valor)
  
  #Mostra os vídeos na tela
  cv2.imshow("video",frame)
  cv2.imshow("Bird eyes",dst)
  cv2.imshow("IMG_BINARIA",mask)

  #Encerra o programa caso a tecla "q" seja pressionada
  if cv2.waitKey(30) == ord('q'):
      break

#Fecha o vídeo e as janelas
video.release()
cv2.destroyAllWindows()

