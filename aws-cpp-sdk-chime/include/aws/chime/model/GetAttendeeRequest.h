﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/chime/Chime_EXPORTS.h>
#include <aws/chime/ChimeRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Chime
{
namespace Model
{

  /**
   */
  class AWS_CHIME_API GetAttendeeRequest : public ChimeRequest
  {
  public:
    GetAttendeeRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<GetAttendeeRequest> Clone() const
    {
      return Aws::MakeUnique<GetAttendeeRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetAttendee"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The Amazon Chime SDK meeting ID.</p>
     */
    inline const Aws::String& GetMeetingId() const{ return m_meetingId; }

    /**
     * <p>The Amazon Chime SDK meeting ID.</p>
     */
    inline bool MeetingIdHasBeenSet() const { return m_meetingIdHasBeenSet; }

    /**
     * <p>The Amazon Chime SDK meeting ID.</p>
     */
    inline void SetMeetingId(const Aws::String& value) { m_meetingIdHasBeenSet = true; m_meetingId = value; }

    /**
     * <p>The Amazon Chime SDK meeting ID.</p>
     */
    inline void SetMeetingId(Aws::String&& value) { m_meetingIdHasBeenSet = true; m_meetingId = std::move(value); }

    /**
     * <p>The Amazon Chime SDK meeting ID.</p>
     */
    inline void SetMeetingId(const char* value) { m_meetingIdHasBeenSet = true; m_meetingId.assign(value); }

    /**
     * <p>The Amazon Chime SDK meeting ID.</p>
     */
    inline GetAttendeeRequest& WithMeetingId(const Aws::String& value) { SetMeetingId(value); return *this;}

    /**
     * <p>The Amazon Chime SDK meeting ID.</p>
     */
    inline GetAttendeeRequest& WithMeetingId(Aws::String&& value) { SetMeetingId(std::move(value)); return *this;}

    /**
     * <p>The Amazon Chime SDK meeting ID.</p>
     */
    inline GetAttendeeRequest& WithMeetingId(const char* value) { SetMeetingId(value); return *this;}


    /**
     * <p>The Amazon Chime SDK attendee ID.</p>
     */
    inline const Aws::String& GetAttendeeId() const{ return m_attendeeId; }

    /**
     * <p>The Amazon Chime SDK attendee ID.</p>
     */
    inline bool AttendeeIdHasBeenSet() const { return m_attendeeIdHasBeenSet; }

    /**
     * <p>The Amazon Chime SDK attendee ID.</p>
     */
    inline void SetAttendeeId(const Aws::String& value) { m_attendeeIdHasBeenSet = true; m_attendeeId = value; }

    /**
     * <p>The Amazon Chime SDK attendee ID.</p>
     */
    inline void SetAttendeeId(Aws::String&& value) { m_attendeeIdHasBeenSet = true; m_attendeeId = std::move(value); }

    /**
     * <p>The Amazon Chime SDK attendee ID.</p>
     */
    inline void SetAttendeeId(const char* value) { m_attendeeIdHasBeenSet = true; m_attendeeId.assign(value); }

    /**
     * <p>The Amazon Chime SDK attendee ID.</p>
     */
    inline GetAttendeeRequest& WithAttendeeId(const Aws::String& value) { SetAttendeeId(value); return *this;}

    /**
     * <p>The Amazon Chime SDK attendee ID.</p>
     */
    inline GetAttendeeRequest& WithAttendeeId(Aws::String&& value) { SetAttendeeId(std::move(value)); return *this;}

    /**
     * <p>The Amazon Chime SDK attendee ID.</p>
     */
    inline GetAttendeeRequest& WithAttendeeId(const char* value) { SetAttendeeId(value); return *this;}

  private:

    Aws::String m_meetingId;
    bool m_meetingIdHasBeenSet = false;

    Aws::String m_attendeeId;
    bool m_attendeeIdHasBeenSet = false;
  };

} // namespace Model
} // namespace Chime
} // namespace Aws
