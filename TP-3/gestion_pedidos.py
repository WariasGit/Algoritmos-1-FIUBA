import csv
import os
AGREGAR ='agregar'
MODIFICAR ='modificar'
ELIMINAR ='eliminar'
LISTAR ='listar'

PEDIDOS ="pedidos.csv"
CLIENTES ="clientes.csv"
AUXILIAR ="auxiliar.csv"

CANTIDAD_DATOS_PARA_AGREGAR =3
CANTIDAD_DATOS_PARA_MODIFICAR =3
CANTIDAD_DATOS_PARA_ELIMINAR =1
CANTIDAD_DATOS_PARA_LISTAR =1
CERO =0
AL_INICIO =0
UNO =1

TOMATE ='T'
LECHUGA ='L'
BROCOLI ='B'
ZANAHORIA ='Z'

INDICE_ID_PEDIDOS =0
INDICE_VERDURA_PEDIDOS =1
INDICE_CANTIDAD_PEDIDOS=2

INDICE_ID_CLIENTES =0
INDICE_NOMBRE_CLIENTES =1

INDICE_CANTIDAD_AGREGAR =0
INDICE_VERDURA_AGREGAR =1
INDICE_CLIENTE_AGREGAR =2

INDICE_ID_MODIFICAR =0
INDICE_CANTIDAD_MODIFICAR =1
INDICE_VERDURA_MODIFICAR =2

INDICE_ID_ELIMINAR =0
INDICE_ID_LISTAR =0



#Recibe un caracter y evalua si se trata de una verdura valida.
def es_verdura_valida(verdura: str) -> bool:
	return verdura==TOMATE or verdura==LECHUGA or verdura==BROCOLI or verdura==ZANAHORIA

#Muestra un mensaje de error generico.
def mostrar_error_argumentos():
	print("Cantidad incorrecta de argumentos o argumentos invalidos.")
	return

#Recibe un comando valido y muestra un mensaje de exito, referido a la accion correspondiente.
def mostrar_mensaje_realizado(comando: str):
	print(f"Se realizo correctamente la accion correspondiente a -{comando}-")

#Muestra el formato a ingresar por consola para agregar.
def mostrar_formato_listar():
	print("Debe ingresar los siguientes datos, separados por espacios:\n")
	print("comando -listar-, id del pedido \n")
	print("Tambien puede ingresar el comando solo y asi listar todos los pedidos \n")

#Muestra el formato a ingresar por consola para eliminar.
def mostrar_formato_eliminar():
	print("Debe ingresar los siguientes datos, separados por espacios: \n")
	print("comando -eliminar-, id del pedido \n")

#Muestra el formato a ingresar por consola para modificar.
def mostrar_formato_modificar():
	print("Debe ingresar los siguientes datos, separados por espacios: \n")
	print("comando -modificar-, id del pedido, cantidad, verdura \n")

#Muestra el formato a ingresar por consola para agregar.
def mostrar_formato_agregar():
	print("Debe ingresar los siguientes datos, separados por espacios: \n")
	print("comando -agregar-, cantidad, verdura, nombre \n")

#Recibe un comando valido. Luego de un error, muestra al usuario como es el formato de los datos/argumentos para el comando.
def mostrar_formato_para_comando(comando: str):
	if comando ==AGREGAR:
		mostrar_formato_agregar()
	elif comando ==MODIFICAR:
		mostrar_formato_modificar()
	elif comando ==ELIMINAR:
		mostrar_formato_eliminar()
	elif comando ==LISTAR:
		mostrar_formato_listar()

#Recibe un id, verifica que corresponda a un pedido realizado.
def es_id_valido(id_ingresado: str) -> bool:
	try:
		archivo_pedidos = open(PEDIDOS)
	except:
		return False

	pedidos = csv.reader(archivo_pedidos, delimiter = ";")
	for fila in pedidos:
		if fila[0] == id_ingresado:
			archivo_pedidos.close()
			return True
	archivo_pedidos.close()
	return False

#Recibe una lista con argumentos ingresados por el usuario y los valida. Devuelve true si son validos, si no, false.
def verificar_para_listar(argumentos_del_comando: list[str]) -> bool:
	return (len(argumentos_del_comando) ==CERO) or (len(argumentos_del_comando) ==CANTIDAD_DATOS_PARA_LISTAR and es_id_valido(argumentos_del_comando[INDICE_ID_LISTAR]))

#Recibe una lista con argumentos ingresados por el usuario y los valida. Devuelve true si son validos, si no, false.
def verificar_para_eliminar(argumentos_del_comando: list[str]) -> bool:
	return len(argumentos_del_comando) ==CANTIDAD_DATOS_PARA_ELIMINAR and es_id_valido(argumentos_del_comando[INDICE_ID_ELIMINAR])

#Recibe una lista con argumentos ingresados por el usuario y los valida. Devuelve true si son validos, si no, false.
def verificar_para_modificar(argumentos_del_comando: list[str]) -> bool:
	return len(argumentos_del_comando) ==CANTIDAD_DATOS_PARA_MODIFICAR and es_id_valido(argumentos_del_comando[INDICE_ID_MODIFICAR]) and int(argumentos_del_comando[INDICE_CANTIDAD_MODIFICAR])>CERO and es_verdura_valida(argumentos_del_comando[INDICE_VERDURA_MODIFICAR])

#Recibe una lista con argumentos ingresados por el usuario y los valida. Devuelve true si son validos, si no, false.
def verificar_para_agregar(argumentos_del_comando: list[str]) -> bool:
	return len(argumentos_del_comando) ==CANTIDAD_DATOS_PARA_AGREGAR and int(argumentos_del_comando[0])>CERO and es_verdura_valida(argumentos_del_comando[INDICE_VERDURA_AGREGAR])


#Recibe un comando valido y verifica que los argumentos sean datos validos para gestionar el pedido.
def son_datos_validos(comando: str, argumentos_del_comando: list[str]) -> bool:
	if comando ==AGREGAR:
		return verificar_para_agregar(argumentos_del_comando)
	elif comando ==MODIFICAR:
		return verificar_para_modificar(argumentos_del_comando)
	elif comando ==ELIMINAR:
		return verificar_para_eliminar(argumentos_del_comando)
	elif comando ==LISTAR:
		return verificar_para_listar(argumentos_del_comando)

#Asigna el id del pedido a ingresar. Devuelve 1 si el archivo no existe o esta vacio. En caso contrario, devuelve el id mas grande +1.
def asignar_id_pedido():
	try:
		archivo_pedidos = open(PEDIDOS)
	except:
		return UNO
	pedidos = csv.reader(archivo_pedidos, delimiter=";")
	if any(pedidos):
		archivo_pedidos.seek(AL_INICIO)
		ultimo_id = CERO
		for linea in pedidos:
			if linea:
				id_actual = int(linea[INDICE_ID_PEDIDOS])
				if id_actual > ultimo_id:
					ultimo_id = id_actual
		archivo_pedidos.close()
		return ultimo_id + UNO
	else:
		archivo_pedidos.close()
		return UNO
	

#Recibe argumentos validos, ingresados por el usuario, le asigna un id y luego carga al archivo, si este no existe, lo crea.
def cargar_pedido(argumentos_del_comando: list[str], id_nuevo: int):
	try:
		archivo_pedidos = open(PEDIDOS, "a")
	except:
		print("No se puede abrir el archivo -pedidos-")
		return 
	pedidos = csv.writer(archivo_pedidos, delimiter =";")
	nuevo_pedido = [id_nuevo, argumentos_del_comando[INDICE_VERDURA_AGREGAR], argumentos_del_comando[INDICE_CANTIDAD_AGREGAR]]
	pedidos.writerow(nuevo_pedido)
	archivo_pedidos.close()

#Recibe argumentos validos. Abre el archivo clientes, si no existe lo crea. Si hay algun error, imprime un mensaje. Carga los datos al archivo.
def cargar_cliente(argumentos_del_comando: list[str], id_nuevo: int):
	try:
		archivo_clientes = open(CLIENTES, "a")
	except:
		print("No se puede abrir el archivo -clientes-")
		return 
	clientes = csv.writer(archivo_clientes, delimiter =";")
	cliente_nuevo = [id_nuevo, argumentos_del_comando[INDICE_CLIENTE_AGREGAR]]
	clientes.writerow(cliente_nuevo)
	archivo_clientes.close()

#Recibe argumentos validos, ingresados por el usuario y modifica el pedido, si este no existe, lo crea.
def cambiar_pedido(argumentos_del_comando: list[str]):
	try:
		archivo_pedidos = open(PEDIDOS)
	except:
		print("No se puede abrir el archivo -pedidos-")
		return 
	try:
		archivo_auxiliar = open(AUXILIAR,"w")
	except:
		archivo_pedidos.close()
		print("No se puede abrir el archivo -auxiliar-")
		return
	lector = csv.reader(archivo_pedidos, delimiter = ";")
	escritor = csv.writer(archivo_auxiliar, delimiter = ";")
	agregado = False
	pedido_a_agregar = [argumentos_del_comando[INDICE_ID_MODIFICAR], argumentos_del_comando[INDICE_VERDURA_MODIFICAR], argumentos_del_comando[INDICE_CANTIDAD_MODIFICAR]]
	for fila in lector:
		if int(fila[INDICE_ID_PEDIDOS]) < int(argumentos_del_comando[INDICE_ID_MODIFICAR]):
			escritor.writerow(fila)
		elif int(fila[INDICE_ID_PEDIDOS]) == int(argumentos_del_comando[INDICE_ID_MODIFICAR]):
			if fila[INDICE_VERDURA_PEDIDOS] == argumentos_del_comando[INDICE_VERDURA_MODIFICAR]:
				fila[INDICE_CANTIDAD_PEDIDOS] = argumentos_del_comando[INDICE_CANTIDAD_MODIFICAR]
				escritor.writerow(fila)
				agregado = True
			else:
				escritor.writerow(fila)
		else:
			if not agregado:
				escritor.writerow(pedido_a_agregar)
				escritor.writerow(fila)
				agregado = True
			else:
				escritor.writerow(fila)
				
	if not agregado:
		escritor.writerow(pedido_a_agregar)
	archivo_pedidos.close()
	archivo_auxiliar.close()
	os.rename(AUXILIAR, PEDIDOS)

#Rescibe un id valido de un pedido a eliminar y lo elimina de la lista de pedidos.
def quitar_de_lista_pedidos(argumentos_del_comando: list[str]):
	try:
		archivo_pedidos = open(PEDIDOS)
	except:
		print("No se puede abrir el archivo -pedidos-")
		return 
	try:
		archivo_auxiliar = open(AUXILIAR,"w")
	except:
		archivo_pedidos.close()
		print("No se puede abrir el archivo -auxiliar-")
		return
	lector = csv.reader(archivo_pedidos, delimiter = ";")
	escritor = csv.writer(archivo_auxiliar, delimiter = ";")
	for fila in lector:
		if fila[INDICE_ID_PEDIDOS] != argumentos_del_comando[INDICE_ID_ELIMINAR]:
			escritor.writerow(fila)
	archivo_pedidos.close()
	archivo_auxiliar.close()
	os.rename(AUXILIAR, PEDIDOS)

#Rescibe un id valido de un pedido a eliminar y elimina al cliente correspondiente a ese pedido.
def quitar_de_lista_clientes(argumentos_del_comando: list[str]):
	try:
		archivo_clientes = open(CLIENTES)
	except:
		print("No se puede abrir el archivo -clientes-")
		return 
	try:
		archivo_auxiliar = open(AUXILIAR,"w")
	except:
		archivo_clientes.close()
		print("No se puede abrir el archivo -auxiliar-")
		return
	lector = csv.reader(archivo_clientes, delimiter=";")
	escritor = csv.writer(archivo_auxiliar, delimiter=";")
	for fila in lector:
		if fila[INDICE_ID_CLIENTES] != argumentos_del_comando[INDICE_ID_ELIMINAR]:
			escritor.writerow(fila)
	archivo_clientes.close()
	archivo_auxiliar.close()
	os.rename(AUXILIAR, CLIENTES)

#Recibe los argumentos validos para el comando. Elimina el pedido de pedidos.csv y de clientes.csv
def quitar_pedido(argumentos_del_comando: list[str]):
	quitar_de_lista_pedidos(argumentos_del_comando)
	quitar_de_lista_clientes(argumentos_del_comando)

#Muestra todos los pedidos del archivo pedidos.csv
def mostrar_todos_los_pedidos():
	try:
		archivo_pedidos = open(PEDIDOS)
	except:
		print("No se puede abrir el archivo -pedidos-")
		return 
	try:
		archivo_clientes = open(CLIENTES)
	except:
		print("No se puede abrir el archivo -clientes-")
		return 
	lector_pedidos = csv.reader(archivo_pedidos, delimiter=";")
	lector_clientes = csv.reader(archivo_clientes, delimiter=";")
	linea_pedido =next(lector_pedidos, None)
	linea_cliente =next(lector_clientes, None)
	while linea_pedido is not None and linea_cliente is not None:
			id_pedido =int(linea_pedido[INDICE_ID_PEDIDOS])
			id_cliente =int(linea_cliente[INDICE_ID_CLIENTES])
			if id_pedido ==id_cliente:
				print(linea_pedido + [linea_cliente[1]])
				linea_pedido =next(lector_pedidos, None)
			else:
				linea_cliente =next(lector_clientes, None)
	archivo_pedidos.close()
	archivo_clientes.close()
    

#Muestra el pedido correspondiente al id recibido.
def mostrar_un_pedido(argumentos_del_comando: list[str]):
	try:
		archivo_pedidos = open(PEDIDOS)
	except:
		print("No se puede abrir el archivo -pedidos-")
		return 
	try:
		archivo_clientes = open(CLIENTES)
	except:
		print("No se puede abrir el archivo -clientes-")
		return 
	lector_pedidos = csv.reader(archivo_pedidos, delimiter=";")
	lector_clientes = csv.reader(archivo_clientes, delimiter=";")
	id_ingresado =int(argumentos_del_comando[INDICE_ID_LISTAR])
	linea_pedido =next(lector_pedidos, None)
	linea_cliente =next(lector_clientes, None)
	while linea_pedido is not None and linea_cliente is not None:
		id_pedido =int(linea_pedido[INDICE_ID_PEDIDOS])
		id_cliente =int(linea_cliente[INDICE_ID_CLIENTES])
		if id_pedido ==id_ingresado:
			if id_cliente ==id_ingresado:
				print(linea_pedido + [linea_cliente[1]])
				linea_pedido =next(lector_pedidos, None)
			else:
				linea_cliente =next(lector_clientes, None)
		else:
			linea_pedido =next(lector_pedidos, None)
	archivo_clientes.close()
	archivo_pedidos.close()

#Recibe el id del pedido a listar y lo muestra. Si no recibe un id, muestra todos los pedidos.
def mostrar_pedidos(argumentos_del_comando: list[str]):
	if not any(argumentos_del_comando):
		mostrar_todos_los_pedidos()
	else:
		mostrar_un_pedido(argumentos_del_comando)

#Recibe un comando valido, valida los argumentos del mismo y agrega el pedido.
def agregar_pedido(comando: str, argumentos_del_comando: list[str]):
	if son_datos_validos(comando, argumentos_del_comando):
		id_nuevo = asignar_id_pedido()
		cargar_pedido(argumentos_del_comando, id_nuevo)
		cargar_cliente(argumentos_del_comando, id_nuevo)
		mostrar_mensaje_realizado(comando)
	else:
		mostrar_error_argumentos()
		mostrar_formato_para_comando(comando)

#Recibe un comando valido, valida los argumentos del mismo y modifica el pedido.
def modificar_pedido(comando: str, argumentos_del_comando: list[str]):
	if son_datos_validos(comando, argumentos_del_comando):
		cambiar_pedido(argumentos_del_comando)
		mostrar_mensaje_realizado(comando)
	else:
		mostrar_error_argumentos()
		mostrar_formato_para_comando(comando)

#Recibe un comando valido, valida los argumentos del mismo y elimina el pedido. 
def eliminar_pedido(comando: str, argumentos_del_comando: list[str]):
	if son_datos_validos(comando, argumentos_del_comando):
		quitar_pedido(argumentos_del_comando)
		mostrar_mensaje_realizado(comando)
	else:
		mostrar_error_argumentos()
		mostrar_formato_para_comando(comando)

##Recibe un comando valido, valida los argumentos del mismo y muestra la lista de pedidos.
def listar_pedidos(comando: str, argumentos_del_comando: list[str]):
	if son_datos_validos(comando, argumentos_del_comando):
		mostrar_pedidos(argumentos_del_comando)
		mostrar_mensaje_realizado(comando)
	else:
		mostrar_error_argumentos()
		mostrar_formato_para_comando(comando)
