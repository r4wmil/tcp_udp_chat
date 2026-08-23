import socket
import sys
import threading
import time

HOST = "127.0.0.1"
PORT = 7777

def recv(sock):
    while True:
        try:
            data = sock.recv(4096)
            if not data:
                print("\n[conn lost]")
                return
            print(data.decode(), end="", flush=True)
        except OSError:
            print("\n[conn error]")
            return


while True:
    try:
        print(f"[connecting to {HOST}:{PORT}]")
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((HOST, PORT))
        print("[connected]")
        threading.Thread(target=recv, args=(sock,), daemon=True).start()
        for line in sys.stdin:
            sock.sendall(line.encode())
        sock.close()
        break
    except (ConnectionRefusedError, ConnectionResetError, BrokenPipeError, OSError):
        print("[server unavailable. retrying in 2 sec...]")
        try:
            sock.close()
        except: pass
        time.sleep(2)
