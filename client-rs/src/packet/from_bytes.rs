use bytes::{Buf, Bytes};
use color_eyre::eyre::Result;
use log::error;
use uuid::Uuid;

use crate::packet::{flags::Flags, FromByte, FromBytes, PacketError};

use super::{version::Version, Packet, PacketTypeState};

impl<T: PacketTypeState> FromBytes for Packet<T> {
    type Error = PacketError;

    fn from_bytes(data: &[u8]) -> Result<Self, PacketError> {
        let mut buf = Bytes::copy_from_slice(data);

        if buf.len() < 22 {
            error!("not enough header bytes");
            return Err(PacketError::NotEnoughHeaderBytes);
        }

        let version = Version::from_byte(buf.get_u8())?;

        let packet_type = buf.get_u8();
        if packet_type != T::ID {
            error!("unexpected packet type");
            return Err(PacketError::InvalidPacketType {
                expected: T::ID,
                found: packet_type,
            });
        }

        let client_id =
            Uuid::from_slice(&buf.copy_to_bytes(16)).map_err(|err| PacketError::UuidError(err))?;

        let flags = Flags::from_bits(buf.get_u8()).ok_or(PacketError::FlagsError)?;

        let payload_len = buf.get_u32_le();

        if buf.remaining() < payload_len as usize {
            error!("not enough payload bytes");
            return Err(PacketError::NotEnoughPayloadBytes);
        }

        let payload = buf.copy_to_bytes(payload_len as usize).to_vec();

        if buf.has_remaining() {
            error!("to many bytes provided");
            return Err(PacketError::ToManyBytes {
                expected: (22 + payload_len) as usize,
                found: buf.len(),
            });
        }

        Ok(Self {
            packet_type: T::default(),
            client_id,
            flags,
            payload_len,
            payload,
        })
    }
}
