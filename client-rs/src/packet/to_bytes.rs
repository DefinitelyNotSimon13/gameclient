use crate::packet::{ToByte, ToBytes};
use bytes::{BufMut, BytesMut};
use log::{debug, error};

use super::{Packet, PacketTypeState, PACKET_VERSION};

impl<T: PacketTypeState> ToBytes for Packet<T> {
    fn to_bytes(&self) -> Vec<u8> {
        // version      | 1
        // type         | 1
        // clientId     | 16
        // flags        | 1
        // payload len  | 4
        // payload      | payllen
        let capacity = 1 + 1 + 16 + 1 + 4 + self.payload.len();
        let mut buf = BytesMut::with_capacity(capacity);

        buf.put_u8(PACKET_VERSION);

        buf.put_u8(self.packet_type.to_byte());

        buf.put_slice(self.client_id.as_bytes());

        buf.put_u8(self.flags.bits());

        buf.put_u32_le(self.payload_len);

        buf.put_slice(&self.payload);

        if buf.capacity() != buf.len() {
            error!("packet wasn't written to buf fully");
            debug!(
                "capacity is {}, but {} bytes were written",
                buf.capacity(),
                buf.len()
            )
        }

        buf.to_vec()
    }
}
