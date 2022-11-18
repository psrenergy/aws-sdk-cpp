﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/cloudtrail/CloudTrail_EXPORTS.h>
#include <aws/cloudtrail/CloudTrailRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace CloudTrail
{
namespace Model
{

  /**
   */
  class AWS_CLOUDTRAIL_API StopImportRequest : public CloudTrailRequest
  {
  public:
    StopImportRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<StopImportRequest> Clone() const
    {
      return Aws::MakeUnique<StopImportRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "StopImport"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p> The ID of the import. </p>
     */
    inline const Aws::String& GetImportId() const{ return m_importId; }

    /**
     * <p> The ID of the import. </p>
     */
    inline bool ImportIdHasBeenSet() const { return m_importIdHasBeenSet; }

    /**
     * <p> The ID of the import. </p>
     */
    inline void SetImportId(const Aws::String& value) { m_importIdHasBeenSet = true; m_importId = value; }

    /**
     * <p> The ID of the import. </p>
     */
    inline void SetImportId(Aws::String&& value) { m_importIdHasBeenSet = true; m_importId = std::move(value); }

    /**
     * <p> The ID of the import. </p>
     */
    inline void SetImportId(const char* value) { m_importIdHasBeenSet = true; m_importId.assign(value); }

    /**
     * <p> The ID of the import. </p>
     */
    inline StopImportRequest& WithImportId(const Aws::String& value) { SetImportId(value); return *this;}

    /**
     * <p> The ID of the import. </p>
     */
    inline StopImportRequest& WithImportId(Aws::String&& value) { SetImportId(std::move(value)); return *this;}

    /**
     * <p> The ID of the import. </p>
     */
    inline StopImportRequest& WithImportId(const char* value) { SetImportId(value); return *this;}

  private:

    Aws::String m_importId;
    bool m_importIdHasBeenSet = false;
  };

} // namespace Model
} // namespace CloudTrail
} // namespace Aws
