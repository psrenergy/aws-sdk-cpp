﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/mediaconvert/MediaConvert_EXPORTS.h>
#include <aws/mediaconvert/MediaConvertRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace MediaConvert
{
namespace Model
{

  /**
   */
  class AWS_MEDIACONVERT_API GetQueueRequest : public MediaConvertRequest
  {
  public:
    GetQueueRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<GetQueueRequest> Clone() const
    {
      return Aws::MakeUnique<GetQueueRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetQueue"; }

    Aws::String SerializePayload() const override;


    /**
     * The name of the queue that you want information about.
     */
    inline const Aws::String& GetName() const{ return m_name; }

    /**
     * The name of the queue that you want information about.
     */
    inline bool NameHasBeenSet() const { return m_nameHasBeenSet; }

    /**
     * The name of the queue that you want information about.
     */
    inline void SetName(const Aws::String& value) { m_nameHasBeenSet = true; m_name = value; }

    /**
     * The name of the queue that you want information about.
     */
    inline void SetName(Aws::String&& value) { m_nameHasBeenSet = true; m_name = std::move(value); }

    /**
     * The name of the queue that you want information about.
     */
    inline void SetName(const char* value) { m_nameHasBeenSet = true; m_name.assign(value); }

    /**
     * The name of the queue that you want information about.
     */
    inline GetQueueRequest& WithName(const Aws::String& value) { SetName(value); return *this;}

    /**
     * The name of the queue that you want information about.
     */
    inline GetQueueRequest& WithName(Aws::String&& value) { SetName(std::move(value)); return *this;}

    /**
     * The name of the queue that you want information about.
     */
    inline GetQueueRequest& WithName(const char* value) { SetName(value); return *this;}

  private:

    Aws::String m_name;
    bool m_nameHasBeenSet = false;
  };

} // namespace Model
} // namespace MediaConvert
} // namespace Aws
