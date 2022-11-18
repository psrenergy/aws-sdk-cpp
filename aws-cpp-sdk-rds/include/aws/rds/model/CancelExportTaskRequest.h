﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/rds/RDS_EXPORTS.h>
#include <aws/rds/RDSRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace RDS
{
namespace Model
{

  /**
   */
  class AWS_RDS_API CancelExportTaskRequest : public RDSRequest
  {
  public:
    CancelExportTaskRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<CancelExportTaskRequest> Clone() const
    {
      return Aws::MakeUnique<CancelExportTaskRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "CancelExportTask"; }

    Aws::String SerializePayload() const override;

  protected:
    void DumpBodyToUrl(Aws::Http::URI& uri ) const override;

  public:

    /**
     * <p>The identifier of the snapshot export task to cancel.</p>
     */
    inline const Aws::String& GetExportTaskIdentifier() const{ return m_exportTaskIdentifier; }

    /**
     * <p>The identifier of the snapshot export task to cancel.</p>
     */
    inline bool ExportTaskIdentifierHasBeenSet() const { return m_exportTaskIdentifierHasBeenSet; }

    /**
     * <p>The identifier of the snapshot export task to cancel.</p>
     */
    inline void SetExportTaskIdentifier(const Aws::String& value) { m_exportTaskIdentifierHasBeenSet = true; m_exportTaskIdentifier = value; }

    /**
     * <p>The identifier of the snapshot export task to cancel.</p>
     */
    inline void SetExportTaskIdentifier(Aws::String&& value) { m_exportTaskIdentifierHasBeenSet = true; m_exportTaskIdentifier = std::move(value); }

    /**
     * <p>The identifier of the snapshot export task to cancel.</p>
     */
    inline void SetExportTaskIdentifier(const char* value) { m_exportTaskIdentifierHasBeenSet = true; m_exportTaskIdentifier.assign(value); }

    /**
     * <p>The identifier of the snapshot export task to cancel.</p>
     */
    inline CancelExportTaskRequest& WithExportTaskIdentifier(const Aws::String& value) { SetExportTaskIdentifier(value); return *this;}

    /**
     * <p>The identifier of the snapshot export task to cancel.</p>
     */
    inline CancelExportTaskRequest& WithExportTaskIdentifier(Aws::String&& value) { SetExportTaskIdentifier(std::move(value)); return *this;}

    /**
     * <p>The identifier of the snapshot export task to cancel.</p>
     */
    inline CancelExportTaskRequest& WithExportTaskIdentifier(const char* value) { SetExportTaskIdentifier(value); return *this;}

  private:

    Aws::String m_exportTaskIdentifier;
    bool m_exportTaskIdentifierHasBeenSet = false;
  };

} // namespace Model
} // namespace RDS
} // namespace Aws
