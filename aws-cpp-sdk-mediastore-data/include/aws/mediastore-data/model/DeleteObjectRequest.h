﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/mediastore-data/MediaStoreData_EXPORTS.h>
#include <aws/mediastore-data/MediaStoreDataRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace MediaStoreData
{
namespace Model
{

  /**
   */
  class AWS_MEDIASTOREDATA_API DeleteObjectRequest : public MediaStoreDataRequest
  {
  public:
    DeleteObjectRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DeleteObjectRequest> Clone() const
    {
      return Aws::MakeUnique<DeleteObjectRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DeleteObject"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The path (including the file name) where the object is stored in the
     * container. Format: &lt;folder name&gt;/&lt;folder name&gt;/&lt;file name&gt;</p>
     */
    inline const Aws::String& GetPath() const{ return m_path; }

    /**
     * <p>The path (including the file name) where the object is stored in the
     * container. Format: &lt;folder name&gt;/&lt;folder name&gt;/&lt;file name&gt;</p>
     */
    inline bool PathHasBeenSet() const { return m_pathHasBeenSet; }

    /**
     * <p>The path (including the file name) where the object is stored in the
     * container. Format: &lt;folder name&gt;/&lt;folder name&gt;/&lt;file name&gt;</p>
     */
    inline void SetPath(const Aws::String& value) { m_pathHasBeenSet = true; m_path = value; }

    /**
     * <p>The path (including the file name) where the object is stored in the
     * container. Format: &lt;folder name&gt;/&lt;folder name&gt;/&lt;file name&gt;</p>
     */
    inline void SetPath(Aws::String&& value) { m_pathHasBeenSet = true; m_path = std::move(value); }

    /**
     * <p>The path (including the file name) where the object is stored in the
     * container. Format: &lt;folder name&gt;/&lt;folder name&gt;/&lt;file name&gt;</p>
     */
    inline void SetPath(const char* value) { m_pathHasBeenSet = true; m_path.assign(value); }

    /**
     * <p>The path (including the file name) where the object is stored in the
     * container. Format: &lt;folder name&gt;/&lt;folder name&gt;/&lt;file name&gt;</p>
     */
    inline DeleteObjectRequest& WithPath(const Aws::String& value) { SetPath(value); return *this;}

    /**
     * <p>The path (including the file name) where the object is stored in the
     * container. Format: &lt;folder name&gt;/&lt;folder name&gt;/&lt;file name&gt;</p>
     */
    inline DeleteObjectRequest& WithPath(Aws::String&& value) { SetPath(std::move(value)); return *this;}

    /**
     * <p>The path (including the file name) where the object is stored in the
     * container. Format: &lt;folder name&gt;/&lt;folder name&gt;/&lt;file name&gt;</p>
     */
    inline DeleteObjectRequest& WithPath(const char* value) { SetPath(value); return *this;}

  private:

    Aws::String m_path;
    bool m_pathHasBeenSet = false;
  };

} // namespace Model
} // namespace MediaStoreData
} // namespace Aws
