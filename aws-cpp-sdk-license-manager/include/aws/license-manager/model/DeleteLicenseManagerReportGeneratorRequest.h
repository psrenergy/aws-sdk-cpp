﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/license-manager/LicenseManager_EXPORTS.h>
#include <aws/license-manager/LicenseManagerRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace LicenseManager
{
namespace Model
{

  /**
   */
  class AWS_LICENSEMANAGER_API DeleteLicenseManagerReportGeneratorRequest : public LicenseManagerRequest
  {
  public:
    DeleteLicenseManagerReportGeneratorRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DeleteLicenseManagerReportGenerator"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>Amazon Resource Name (ARN) of the report generator to be deleted.</p>
     */
    inline const Aws::String& GetLicenseManagerReportGeneratorArn() const{ return m_licenseManagerReportGeneratorArn; }

    /**
     * <p>Amazon Resource Name (ARN) of the report generator to be deleted.</p>
     */
    inline bool LicenseManagerReportGeneratorArnHasBeenSet() const { return m_licenseManagerReportGeneratorArnHasBeenSet; }

    /**
     * <p>Amazon Resource Name (ARN) of the report generator to be deleted.</p>
     */
    inline void SetLicenseManagerReportGeneratorArn(const Aws::String& value) { m_licenseManagerReportGeneratorArnHasBeenSet = true; m_licenseManagerReportGeneratorArn = value; }

    /**
     * <p>Amazon Resource Name (ARN) of the report generator to be deleted.</p>
     */
    inline void SetLicenseManagerReportGeneratorArn(Aws::String&& value) { m_licenseManagerReportGeneratorArnHasBeenSet = true; m_licenseManagerReportGeneratorArn = std::move(value); }

    /**
     * <p>Amazon Resource Name (ARN) of the report generator to be deleted.</p>
     */
    inline void SetLicenseManagerReportGeneratorArn(const char* value) { m_licenseManagerReportGeneratorArnHasBeenSet = true; m_licenseManagerReportGeneratorArn.assign(value); }

    /**
     * <p>Amazon Resource Name (ARN) of the report generator to be deleted.</p>
     */
    inline DeleteLicenseManagerReportGeneratorRequest& WithLicenseManagerReportGeneratorArn(const Aws::String& value) { SetLicenseManagerReportGeneratorArn(value); return *this;}

    /**
     * <p>Amazon Resource Name (ARN) of the report generator to be deleted.</p>
     */
    inline DeleteLicenseManagerReportGeneratorRequest& WithLicenseManagerReportGeneratorArn(Aws::String&& value) { SetLicenseManagerReportGeneratorArn(std::move(value)); return *this;}

    /**
     * <p>Amazon Resource Name (ARN) of the report generator to be deleted.</p>
     */
    inline DeleteLicenseManagerReportGeneratorRequest& WithLicenseManagerReportGeneratorArn(const char* value) { SetLicenseManagerReportGeneratorArn(value); return *this;}

  private:

    Aws::String m_licenseManagerReportGeneratorArn;
    bool m_licenseManagerReportGeneratorArnHasBeenSet = false;
  };

} // namespace Model
} // namespace LicenseManager
} // namespace Aws
