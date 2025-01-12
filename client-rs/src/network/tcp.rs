use tokio::net::TcpStream;

use crate::packet::{Packet, ToBytes};

use super::NetworkError;
use tokio::io::AsyncWriteExt;

pub struct TcpConn {
    stream: TcpStream,
}

impl TcpConn {
    pub async fn new(addr: &str) -> Result<TcpConn, NetworkError> {
        let stream = TcpStream::connect(addr)
            .await
            .map_err(|e| NetworkError::FailedToConnect { e })?;
        Ok(Self { stream })
    }

    pub async fn send_packet(&mut self, packet: &impl ToBytes) -> Result<(), NetworkError> {
        self.stream
            .write_all(&packet.to_bytes())
            .await
            .map_err(|e| NetworkError::FailedToWrite { e })?;
        Ok(())
    }

    pub async fn wait_for_packet(&self, buf: &mut [u8]) -> Result<usize, NetworkError> {
        self.stream
            .readable()
            .await
            .map_err(|e| NetworkError::FailedToRetrieve { e })?;

        match self.stream.try_read(buf) {
            Ok(0) => Err(NetworkError::NoBytesRead),
            Ok(n) => Ok(n),
            Err(e) => Err(NetworkError::FailedToRetrieve { e }),
        }
    }
}
