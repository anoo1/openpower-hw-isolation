// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "common_types.hpp"
#include "openpower_guard_interface.hpp"

#include <xyz/openbmc_project/Association/Definitions/server.hpp>
#include <xyz/openbmc_project/HardwareIsolation/Entry/server.hpp>
#include <xyz/openbmc_project/Object/Delete/server.hpp>
#include <xyz/openbmc_project/Time/EpochTime/server.hpp>

#include <tuple>
#include <vector>

namespace hw_isolation
{
namespace entry
{

using EntryInterface =
    sdbusplus::xyz::openbmc_project::HardwareIsolation::server::Entry;
using EntryId = uint32_t;
using EntryRecordId = uint32_t;
using EntrySeverity =
    sdbusplus::xyz::openbmc_project::HardwareIsolation::server::Entry::Type;
using EntryResolved = bool;

using AssociationDefInterface =
    sdbusplus::xyz::openbmc_project::Association::server::Definitions;
using AsscDefFwdType = std::string;
using AsscDefRevType = std::string;
using AssociatedObjPat = std::string;
using AssociationDef =
    std::vector<std::tuple<AsscDefFwdType, AsscDefRevType, AssociatedObjPat>>;

using EpochTime = sdbusplus::xyz::openbmc_project::Time::server::EpochTime;

using DeleteInterface = sdbusplus::xyz::openbmc_project::Object::server::Delete;

/**
 * @class Entry
 *
 * @brief Hardware isolation entry implementation
 *
 * @details Implemented the below interfaces
 *          xyz.openbmc_project.HardwareIsolation.Entry
 *          xyz.openbmc_project.Association.Definitions
 *          xyz.openbmc_project.Time.EpochTime
 *          xyz.openbmc_project.Object.Delete
 *
 */
class Entry :
    public type::ServerObject<EntryInterface, AssociationDefInterface,
                              EpochTime, DeleteInterface>
{
  public:
    Entry() = delete;
    Entry(const Entry&) = delete;
    Entry& operator=(const Entry&) = delete;
    Entry(Entry&&) = delete;
    Entry& operator=(Entry&&) = delete;
    virtual ~Entry() = default;

    /** @brief Constructor to put object onto bus at a dbus path.
     *
     *  @param[in] bus - Bus to attach with dbus entry object path.
     *  @param[in] objPath - Entry dbus object path to attach.
     *  @param[in] entryId - the dbus entry id.
     *  @param[in] entryRecordId - the isolated hardware record id.
     *  @param[in] isolatedHwSeverity - the severity hardware isolation.
     *  @param[in] entryIsResolved - the status of hardware isolation.
     *  @param[in] associationDef - the association to hold other dbus
     *                              object path along with entry object.
     *  @param[in] entityPath - the entry entity path of hardware
     */
    Entry(sdbusplus::bus::bus& bus, const std::string& objPath,
          const EntryId entryId, const EntryRecordId entryRecordId,
          const EntrySeverity isolatedHwSeverity,
          const EntryResolved entryIsResolved,
          const AssociationDef& associationDef,
          const openpower_guard::EntityPath& entityPath);

    /**
     *  @brief Mark this object as resolved
     *
     *  @return NULL
     *
     */
    void delete_() override;

    /**
     * @brief Used get the entity path of isolated hardware.
     */
    openpower_guard::EntityPath getEntityPath() const;

    /**
     * @brief Used get the record id of isolated hardware.
     */
    EntryRecordId getEntryRecId() const;

  private:
    /** @brief Attached bus connection */
    sdbusplus::bus::bus& _bus;

    /** @brief The id of isolated hardware dbus entry */
    EntryId _entryId;

    /** @brief The record id of isolated hardware dbus entry
     *
     *  @note This record id is shared between BMC and Host applications.
     */
    EntryRecordId _entryRecordId;

    /** @brief The entity path of this entry */
    openpower_guard::EntityPath _entityPath;

}; // end of Entry class

namespace utils
{

/**
 * @brief Helper function to get EntrySeverity based on
 *        the given GardType
 *
 * @param[in] gardType openpower gard type
 *
 * @return EntrySeverity on success
 *         Empty optional on failure
 *
 * @note This function will return EntrySeverity::Warning
 * if the given GardType is not found in conversion switch block
 */
std::optional<EntrySeverity>
    getEntrySeverityType(const openpower_guard::GardType gardType);

/**
 * @breif Helper function to get the GardType based on
 *        the given EntrySeverity
 *
 * @param[in] severity hardware isloation entry severity
 *
 * @return GardType on success
 *         Empty optional on failure
 *
 * @note This function will return GardType::GARD_Predictive
 * if the given EntrySeverity is not found in conversion switch block
 *
 */
std::optional<openpower_guard::GardType>
    getGuardType(const EntrySeverity severity);

} // namespace utils
} // namespace entry
} // namespace hw_isolation
