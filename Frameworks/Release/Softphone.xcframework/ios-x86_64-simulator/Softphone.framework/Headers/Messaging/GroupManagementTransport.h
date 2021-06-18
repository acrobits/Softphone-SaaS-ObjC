//
//  GroupManagementTransport.h
//  libsoftphone
//
//  Created by Stanislav Kutil on 18/02/2019.
//
#pragma once

#include "ali/ali_string.h"
#include "ali/ali_array.h"
#include "ali/ali_mime_content_type.h"
#include "ali/ali_filesystem2.h"
#include "ali/ali_callback.h"
#include "Softphone/EventHistory/EventHistory.h"

namespace Softphone::Messaging::GroupManagementTransport
{
    class Interface;
    
    typedef ali::array<EventHistory::StreamParty> GroupChatMembers;
    
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct GroupChatAvatar
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        ali::string checksum;
        ali::mime::content_type contentType;
        ali::filesystem2::path path;
        
        GroupChatAvatar() = default;
        GroupChatAvatar(GroupChatAvatar const& other) = default;
        GroupChatAvatar(GroupChatAvatar && other) = default;
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct GroupChatProperties
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        inline static ali::string_literal const NAME{"name"};
        inline static ali::string_literal const DESCRIPTION{"desc"};
        inline static ali::string_literal const AVATAR{"avatar"};
        inline static ali::string_literal const PROPERTIES_EDITABLE_BY_ADMIN_ONLY{"props_editable_by_admins_only"};
        
        ali::string name;
        ali::string description;
        GroupChatAvatar avatar;
        ali::optional<bool> propertiesEditableByAdminsOnly;
        ali::array_set<ali::string> propertiesToUpdate;
        
        GroupChatProperties() = default;
        GroupChatProperties(GroupChatProperties const& other) = default;
        GroupChatProperties(GroupChatProperties && other) = default;
    };
    
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct GroupChatMetadata
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        GroupChatMetadata() = default;
        GroupChatMetadata(GroupChatMetadata const& other) = default;
        GroupChatMetadata(GroupChatMetadata && other) = default;
        
        GroupChatMembers members;
        GroupChatProperties properties;
    };
    
    typedef ali::callback<void(Interface&, bool, ali::string const&)> CreateGroupChatResultCallback;
    typedef ali::callback<void(Interface&, bool)> UpdateGroupChatResultCallback;
    typedef ali::callback<void(Interface&, bool)> FetchStreamInfoResultCallback;
    typedef ali::callback<void(Interface&, bool)> AddMembersResultCallback;
    typedef ali::callback<void(Interface&, bool)> RemoveMembersResultCallback;
    typedef ali::callback<void(Interface&, bool)> UpdateMembersResultCallback;

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    class Interface
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
    public:
        virtual ~Interface() {};
        
        /** @brief informs about the extent of group management support
         * @retval Returns true if complete set of methods is supported, false if only the unmanaged group streams are created  on the server ad-hoc
         */
        virtual bool isGroupManagementSupported() const = 0;
        
        /** @brief gets the error message for the last finished request
         * @retval the last error message
         */
        virtual ali::string getErrorMessage() const = 0;
        
        /** @brief creates a group stream
         * @param data The list of properties and members
         * @param cb Callback method which is called after the method finishes
         * @retval RequestId that can be passed to cancel method
         */
        virtual int createGroupChat(GroupChatMetadata data, CreateGroupChatResultCallback cb) = 0;
        
        /** @brief updates a group stream
         * @param stream The stream to update
         * @param props The stream properties to update
         * @param cb Callback method which is called after the method finishes
         * @retval RequestId that can be passed to cancel method
         */
        virtual int updateGroupChat(EventHistory::EventStream::Pointer stream, GroupChatProperties props, UpdateGroupChatResultCallback cb) = 0;
        
        /** @brief adds members to a group stream
         * @param stream The stream to add members to
         * @param members The list of members to add
         * @param cb Callback method which is called after the method finishes
         * @retval RequestId that can be passed to cancel method
         */
        virtual int addMembers(EventHistory::EventStream::Pointer stream, GroupChatMembers members, AddMembersResultCallback cb) = 0;
        
        /** @brief removes members from a group stream
         * @param stream The stream to remove members from
         * @param partyIndices The indices of the stream parties to remove
         * @param cb Callback method which is called after the method finishes
         * @retval RequestId that can be passed to cancel method
         */
        virtual int removeMembers(EventHistory::EventStream::Pointer stream, ali::array_set<int> const& partyIndices, RemoveMembersResultCallback cb) = 0;

        /** @brief updates member rights in a group stream
         * @param stream The stream to update members in
         * @param partyIndices The indices of the stream parties to update
         * @param cb Callback method which is called after the method finishes
         * @retval RequestId that can be passed to cancel method
         */
        virtual int updateMembers(EventHistory::EventStream::Pointer stream, GroupChatMembers const& members, UpdateMembersResultCallback cb) = 0;

        
        /** @brief cancel an asynchronous request
         * @param requestId The request to cancel
         * @retval true if request was cancelled, false if it was not found
         */
        virtual bool cancel(int requestId) = 0;
        
        /** @brief cancel an asynchronous request
         * @retval true if there is an asynchronous request running
         */
        virtual bool isPending() const = 0;
    };    
}
