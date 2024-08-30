import sys
from gestion_pedidos import agregar_pedido, modificar_pedido, eliminar_pedido, listar_pedidos, mostrar_error_argumentos

AGREGAR ='agregar'
MODIFICAR ='modificar'
ELIMINAR ='eliminar'
LISTAR ='listar'
VACIO =0

#Devuelve los argumentos ingresados por el usuario, que comienzan en la posicion 1.
def obtener_argumentos_de_linea_de_comandos():
    return sys.argv[1:]

def gestionar_comando_del_usuario(comando: str, argumentos_del_comando: list[str]):
    if comando ==AGREGAR:
        agregar_pedido(comando, argumentos_del_comando)
    elif comando ==MODIFICAR:
        modificar_pedido(comando, argumentos_del_comando)
    elif comando ==ELIMINAR:
        eliminar_pedido(comando, argumentos_del_comando)
    elif comando ==LISTAR:
        listar_pedidos(comando, argumentos_del_comando)
    else:
        print("Uso: python verduleria.py <comando>")
        print(f"Comando no reconocido: {comando}")
        return


def main():
    argumentos =obtener_argumentos_de_linea_de_comandos()
    if len(argumentos)>VACIO:
        comando =argumentos[0]
        argumentos_del_comando =argumentos[1:]
        gestionar_comando_del_usuario(comando, argumentos_del_comando)
    else:
        mostrar_error_argumentos()

if __name__ == '__main__':
    main()
