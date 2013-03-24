/*
 * Copyright (c) 2012, Nasel
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef TINS_DOT1Q_H
#define TINS_DOT1Q_H

#include "pdu.h"
#include "endianness.h"
#include "small_uint.h"

namespace Tins {
class Dot1Q : public PDU {
public:
    /**
     * This PDU's flag.
     */
    static const PDU::PDUType pdu_flag = PDU::DOT1Q;
    /**
     * Default constructor
     */
    Dot1Q();

    /**
     * \brief Constructor which creates an Dot1Q object from a buffer and 
     * adds all identifiable PDUs found in the buffer as children of this 
     * one.
     * 
     * \param buffer The buffer from which this PDU will be constructed.
     * \param total_sz The total size of the buffer.
     */
    Dot1Q(const uint8_t *buffer, uint32_t total_sz);

    // Getters

    /**
     * \brief Returns the header size.
     *
     * This metod overrides PDU::header_size. \sa PDU::header_size
     */
    uint32_t header_size() const;

    /**
     * \brief Returns the frame's trailer size.
     * \return The trailer's size.
     */
    uint32_t trailer_size() const;

    /**
     *  \brief Getter for the priority field.
     *  \return The stored priority field value.
     */
    small_uint<3> priority() const {
        return _header.priority;
    }

    /**
     *  \brief Getter for the cfi field.
     *  \return The stored cfi field value.
     */
    small_uint<1> cfi() const {
        return _header.cfi;
    }

    /**
     *  \brief Getter for the id field.
     *  \return The stored id field value.
     */
    small_uint<12> id() const {
        return _header.idL | (_header.idH << 8);
    }

    /**
     *  \brief Getter for the payload type field.
     *  \return The stored type field value.
     */
    uint16_t payload_type() const {
        return Endian::be_to_host(_header.type);
    }

    /**
     * \brief Getter for the PDU's type.
     * \sa PDU::pdu_type
     */
    PDUType pdu_type() const { return pdu_flag; }
    
    /**
     * \sa PDU::clone
     */
    Dot1Q *clone() const {
        return new Dot1Q(*this);
    }

    // Setters

    /**
     *  \brief Setter for the priority field.
     *  \param new_priority The new priority field value.
     */
    void priority(small_uint<3> new_priority);

    /**
     *  \brief Setter for the cfi field.
     *  \param new_cfi The new cfi field value.
     */
    void cfi(small_uint<1> new_cfi);

    /**
     *  \brief Setter for the id field.
     *  \param new_id The new id field value.
     */
    void id(small_uint<12> new_id);

    /**
     *  \brief Setter for the payload type field.
     *  \param new_type The new type field value.
     */
    void payload_type(uint16_t new_type);
private:
    void write_serialization(uint8_t *buffer, uint32_t total_sz, const PDU *parent);

    struct dot1q_hdr {
        #if TINS_IS_BIG_ENDIAN
            uint16_t priority:3,
                    cfi:1
                    id:12;
            uint16_t type;
        #else
            uint16_t idH:4,
                    cfi:1,
                    priority:3,
                    idL:8;
            uint16_t type;
        #endif
    };
    
    dot1q_hdr _header;
};
}

#endif // TINS_DOT1Q_H