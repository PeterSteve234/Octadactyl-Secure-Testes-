import socket 
import os 

NETLNK_USER = 31
BUFFER_SIZE = 65536


def main():
  
  sock = socket.socket(socket.AF_NETLINK, socket.SOCK_DGRAM, NETLINK_USER)
  sock.bind((os.getpid(), 0)

  print("[User-Space Daemon] Escutando mensagens do kernel...")


while True:
  try:
    
     data, (nl_pid, _) = sock.recvfrom(BUFFER_SIZE)
     mensagem = data.decode("UTF-8", errors="Ignore")
     print(f"[ALERTA LSM] {mensagem}")
  
  except keyboardInterrupt:
      print("encerrando sistema...")
      break
    
  except  Exception as e:
      print(f"[ERRO] {e}")


if __name__ == "__main__":
    main()

