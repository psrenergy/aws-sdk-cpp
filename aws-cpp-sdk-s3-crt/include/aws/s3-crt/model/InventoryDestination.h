﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/s3-crt/S3Crt_EXPORTS.h>
#include <aws/s3-crt/model/InventoryS3BucketDestination.h>
#include <utility>

namespace Aws
{
namespace Utils
{
namespace Xml
{
  class XmlNode;
} // namespace Xml
} // namespace Utils
namespace S3Crt
{
namespace Model
{

  /**
   * <p>Specifies the inventory configuration for an Amazon S3 bucket.</p><p><h3>See
   * Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/s3-2006-03-01/InventoryDestination">AWS
   * API Reference</a></p>
   */
  class AWS_S3CRT_API InventoryDestination
  {
  public:
    InventoryDestination();
    InventoryDestination(const Aws::Utils::Xml::XmlNode& xmlNode);
    InventoryDestination& operator=(const Aws::Utils::Xml::XmlNode& xmlNode);

    void AddToNode(Aws::Utils::Xml::XmlNode& parentNode) const;


    /**
     * <p>Contains the bucket name, file format, bucket owner (optional), and prefix
     * (optional) where inventory results are published.</p>
     */
    inline const InventoryS3BucketDestination& GetS3BucketDestination() const{ return m_s3BucketDestination; }

    /**
     * <p>Contains the bucket name, file format, bucket owner (optional), and prefix
     * (optional) where inventory results are published.</p>
     */
    inline bool S3BucketDestinationHasBeenSet() const { return m_s3BucketDestinationHasBeenSet; }

    /**
     * <p>Contains the bucket name, file format, bucket owner (optional), and prefix
     * (optional) where inventory results are published.</p>
     */
    inline void SetS3BucketDestination(const InventoryS3BucketDestination& value) { m_s3BucketDestinationHasBeenSet = true; m_s3BucketDestination = value; }

    /**
     * <p>Contains the bucket name, file format, bucket owner (optional), and prefix
     * (optional) where inventory results are published.</p>
     */
    inline void SetS3BucketDestination(InventoryS3BucketDestination&& value) { m_s3BucketDestinationHasBeenSet = true; m_s3BucketDestination = std::move(value); }

    /**
     * <p>Contains the bucket name, file format, bucket owner (optional), and prefix
     * (optional) where inventory results are published.</p>
     */
    inline InventoryDestination& WithS3BucketDestination(const InventoryS3BucketDestination& value) { SetS3BucketDestination(value); return *this;}

    /**
     * <p>Contains the bucket name, file format, bucket owner (optional), and prefix
     * (optional) where inventory results are published.</p>
     */
    inline InventoryDestination& WithS3BucketDestination(InventoryS3BucketDestination&& value) { SetS3BucketDestination(std::move(value)); return *this;}

  private:

    InventoryS3BucketDestination m_s3BucketDestination;
    bool m_s3BucketDestinationHasBeenSet = false;
  };

} // namespace Model
} // namespace S3Crt
} // namespace Aws
