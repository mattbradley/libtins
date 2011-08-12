/*
 * libtins is a net packet wrapper library for crafting and 
 * interpreting sniffed packets.
 * 
 * Copyright (C) 2011 Nasel
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __PDU_H
#define __PDU_H


#include <stdint.h>
#include "packetsender.h"

namespace Tins {

    class PacketSender;

    /** \brief PDU is the base class for protocol data units.
     * 
     * Every PDU implementation must inherit this one. PDUs can be serialized,
     * therefore allowing a PacketSender to send them through sockets. PDUs
     * are created upwards: upper layers will be children of the lower ones.
     * Each PDU must provide its flag identifier. This will be most likely added
     * to its parent's data, hence it should be a valid identifier. For example,
     * IP should provide IPPROTO_IP.
     */
    class PDU {
    public:
        /** \brief PDU constructor
         *
         * Must be called by subclasses in their constructors.
         * \param flag The flag identifier for the subclass' PDU.
         * \param next_pdu The child PDU. Can be obviated.
         */
        PDU(uint32_t flag, PDU *next_pdu = 0);
        virtual ~PDU();

        /** \brief The header's size
         */
        virtual uint32_t header_size() const = 0;
        
        /** \brief Trailer's size.
         * 
         * Some protocols require a trailer(like Ethernet). This defaults to 0.
         */
        virtual uint32_t trailer_size() const { return 0; }
        
        /** \brief The whole chain of PDU's size, including this one.
         * 
         * Returns the sum of this and all children PDUs' size.
         */
        uint32_t size() const;
        
        /** \brief This PDU's type flag identifier. 
         * 
         */
        inline uint32_t flag() const { return _flag; }
        
        /** \brief The child PDU.
         */
        inline const PDU *inner_pdu() const { return _inner_pdu; }

        /** \brief Sets the flag identifier.
         */
        void flag(uint32_t new_flag);
        
        /** \brief Sets the child PDU.
         * 
         * \param next_pdu The new child PDU.
         * When setting a new inner_pdu, the instance takesownership of 
         * the object, therefore deleting it when it's no longer required.
         */
        void inner_pdu(PDU *next_pdu);

        
        /** \brief Serializes the whole chain of PDU's, including this one. 
         * 
         * \param sz The size of the buffer must be returned through this parameter.
         * The buffer returned must be deleted by the user using 
         * operator delete[]. 
         */
        uint8_t *serialize(uint32_t &sz);

        /** \brief Send the stack of PDUs through a PacketSender.
         * 
         * This method will be called only for the PDU on the bottom of the stack,
         * therefore it should only implement this method if it can be sent.
         * PacketSender implements specific methods to send packets which start
         * on every valid TCP/IP stack layer; this should only be a proxy for
         * those methods.
         * \param sender The PacketSender which will send the packet.
         */
        virtual bool send(PacketSender* sender) { return false; }
    protected:
        /* Serialize this PDU storing the result in buffer. */
        void serialize(uint8_t *buffer, uint32_t total_sz, PDU *parent);

        /** \brief Serialices this TCP PDU.
         * 
         * Each PDU must override this method and implement it's own
         * serialization.
         * \param buffer The buffer in which the PDU will be serialized.
         * \param total_sz The size available in the buffer.
         * \param parent The PDU that's one level below this one on the stack. Might be 0.
         */
        virtual void write_serialization(uint8_t *buffer, uint32_t total_sz, PDU *parent) = 0;
    private:
        uint32_t _flag;
        PDU *_inner_pdu;
    };
};

#endif
