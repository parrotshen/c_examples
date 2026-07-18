# Install libtirpc and rpcbind
sudo apt install libtirpc-dev
sudo apt install rpcbind
sudo systemctl start rpcbind
sudo systemctl status rpcbind
