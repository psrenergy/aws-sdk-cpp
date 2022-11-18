﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/core/utils/Outcome.h>
#include <aws/core/auth/AWSAuthSigner.h>
#include <aws/core/client/CoreErrors.h>
#include <aws/core/client/RetryStrategy.h>
#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpResponse.h>
#include <aws/core/http/HttpClientFactory.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/core/utils/threading/Executor.h>
#include <aws/core/utils/DNS.h>
#include <aws/core/utils/logging/LogMacros.h>
#include <aws/core/utils/logging/ErrorMacros.h>

#include <aws/robomaker/RoboMakerClient.h>
#include <aws/robomaker/RoboMakerErrorMarshaller.h>
#include <aws/robomaker/RoboMakerEndpointProvider.h>
#include <aws/robomaker/model/BatchDeleteWorldsRequest.h>
#include <aws/robomaker/model/BatchDescribeSimulationJobRequest.h>
#include <aws/robomaker/model/CancelSimulationJobRequest.h>
#include <aws/robomaker/model/CancelSimulationJobBatchRequest.h>
#include <aws/robomaker/model/CancelWorldExportJobRequest.h>
#include <aws/robomaker/model/CancelWorldGenerationJobRequest.h>
#include <aws/robomaker/model/CreateRobotApplicationRequest.h>
#include <aws/robomaker/model/CreateRobotApplicationVersionRequest.h>
#include <aws/robomaker/model/CreateSimulationApplicationRequest.h>
#include <aws/robomaker/model/CreateSimulationApplicationVersionRequest.h>
#include <aws/robomaker/model/CreateSimulationJobRequest.h>
#include <aws/robomaker/model/CreateWorldExportJobRequest.h>
#include <aws/robomaker/model/CreateWorldGenerationJobRequest.h>
#include <aws/robomaker/model/CreateWorldTemplateRequest.h>
#include <aws/robomaker/model/DeleteRobotApplicationRequest.h>
#include <aws/robomaker/model/DeleteSimulationApplicationRequest.h>
#include <aws/robomaker/model/DeleteWorldTemplateRequest.h>
#include <aws/robomaker/model/DescribeRobotApplicationRequest.h>
#include <aws/robomaker/model/DescribeSimulationApplicationRequest.h>
#include <aws/robomaker/model/DescribeSimulationJobRequest.h>
#include <aws/robomaker/model/DescribeSimulationJobBatchRequest.h>
#include <aws/robomaker/model/DescribeWorldRequest.h>
#include <aws/robomaker/model/DescribeWorldExportJobRequest.h>
#include <aws/robomaker/model/DescribeWorldGenerationJobRequest.h>
#include <aws/robomaker/model/DescribeWorldTemplateRequest.h>
#include <aws/robomaker/model/GetWorldTemplateBodyRequest.h>
#include <aws/robomaker/model/ListRobotApplicationsRequest.h>
#include <aws/robomaker/model/ListSimulationApplicationsRequest.h>
#include <aws/robomaker/model/ListSimulationJobBatchesRequest.h>
#include <aws/robomaker/model/ListSimulationJobsRequest.h>
#include <aws/robomaker/model/ListTagsForResourceRequest.h>
#include <aws/robomaker/model/ListWorldExportJobsRequest.h>
#include <aws/robomaker/model/ListWorldGenerationJobsRequest.h>
#include <aws/robomaker/model/ListWorldTemplatesRequest.h>
#include <aws/robomaker/model/ListWorldsRequest.h>
#include <aws/robomaker/model/RestartSimulationJobRequest.h>
#include <aws/robomaker/model/StartSimulationJobBatchRequest.h>
#include <aws/robomaker/model/TagResourceRequest.h>
#include <aws/robomaker/model/UntagResourceRequest.h>
#include <aws/robomaker/model/UpdateRobotApplicationRequest.h>
#include <aws/robomaker/model/UpdateSimulationApplicationRequest.h>
#include <aws/robomaker/model/UpdateWorldTemplateRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::RoboMaker;
using namespace Aws::RoboMaker::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* RoboMakerClient::SERVICE_NAME = "robomaker";
const char* RoboMakerClient::ALLOCATION_TAG = "RoboMakerClient";

RoboMakerClient::RoboMakerClient(const RoboMaker::RoboMakerClientConfiguration& clientConfiguration,
                                 std::shared_ptr<RoboMakerEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RoboMakerErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

RoboMakerClient::RoboMakerClient(const AWSCredentials& credentials,
                                 std::shared_ptr<RoboMakerEndpointProviderBase> endpointProvider,
                                 const RoboMaker::RoboMakerClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RoboMakerErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

RoboMakerClient::RoboMakerClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                 std::shared_ptr<RoboMakerEndpointProviderBase> endpointProvider,
                                 const RoboMaker::RoboMakerClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RoboMakerErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  RoboMakerClient::RoboMakerClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RoboMakerErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<RoboMakerEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

RoboMakerClient::RoboMakerClient(const AWSCredentials& credentials,
                                 const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RoboMakerErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<RoboMakerEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

RoboMakerClient::RoboMakerClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                 const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<RoboMakerErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<RoboMakerEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
RoboMakerClient::~RoboMakerClient()
{
}

std::shared_ptr<RoboMakerEndpointProviderBase>& RoboMakerClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void RoboMakerClient::init(const RoboMaker::RoboMakerClientConfiguration& config)
{
  AWSClient::SetServiceClientName("RoboMaker");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void RoboMakerClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

BatchDeleteWorldsOutcome RoboMakerClient::BatchDeleteWorlds(const BatchDeleteWorldsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchDeleteWorlds, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchDeleteWorlds, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/batchDeleteWorlds");
  return BatchDeleteWorldsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchDeleteWorldsOutcomeCallable RoboMakerClient::BatchDeleteWorldsCallable(const BatchDeleteWorldsRequest& request) const
{
  std::shared_ptr<BatchDeleteWorldsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< BatchDeleteWorldsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->BatchDeleteWorlds(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::BatchDeleteWorldsAsync(const BatchDeleteWorldsRequest& request, const BatchDeleteWorldsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<BatchDeleteWorldsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, BatchDeleteWorlds(*pRequest), context);
    } );
}

BatchDescribeSimulationJobOutcome RoboMakerClient::BatchDescribeSimulationJob(const BatchDescribeSimulationJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchDescribeSimulationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchDescribeSimulationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/batchDescribeSimulationJob");
  return BatchDescribeSimulationJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

BatchDescribeSimulationJobOutcomeCallable RoboMakerClient::BatchDescribeSimulationJobCallable(const BatchDescribeSimulationJobRequest& request) const
{
  std::shared_ptr<BatchDescribeSimulationJobRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< BatchDescribeSimulationJobOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->BatchDescribeSimulationJob(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::BatchDescribeSimulationJobAsync(const BatchDescribeSimulationJobRequest& request, const BatchDescribeSimulationJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<BatchDescribeSimulationJobRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, BatchDescribeSimulationJob(*pRequest), context);
    } );
}

CancelSimulationJobOutcome RoboMakerClient::CancelSimulationJob(const CancelSimulationJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CancelSimulationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CancelSimulationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/cancelSimulationJob");
  return CancelSimulationJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CancelSimulationJobOutcomeCallable RoboMakerClient::CancelSimulationJobCallable(const CancelSimulationJobRequest& request) const
{
  std::shared_ptr<CancelSimulationJobRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CancelSimulationJobOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CancelSimulationJob(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::CancelSimulationJobAsync(const CancelSimulationJobRequest& request, const CancelSimulationJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CancelSimulationJobRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CancelSimulationJob(*pRequest), context);
    } );
}

CancelSimulationJobBatchOutcome RoboMakerClient::CancelSimulationJobBatch(const CancelSimulationJobBatchRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CancelSimulationJobBatch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CancelSimulationJobBatch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/cancelSimulationJobBatch");
  return CancelSimulationJobBatchOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CancelSimulationJobBatchOutcomeCallable RoboMakerClient::CancelSimulationJobBatchCallable(const CancelSimulationJobBatchRequest& request) const
{
  std::shared_ptr<CancelSimulationJobBatchRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CancelSimulationJobBatchOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CancelSimulationJobBatch(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::CancelSimulationJobBatchAsync(const CancelSimulationJobBatchRequest& request, const CancelSimulationJobBatchResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CancelSimulationJobBatchRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CancelSimulationJobBatch(*pRequest), context);
    } );
}

CancelWorldExportJobOutcome RoboMakerClient::CancelWorldExportJob(const CancelWorldExportJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CancelWorldExportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CancelWorldExportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/cancelWorldExportJob");
  return CancelWorldExportJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CancelWorldExportJobOutcomeCallable RoboMakerClient::CancelWorldExportJobCallable(const CancelWorldExportJobRequest& request) const
{
  std::shared_ptr<CancelWorldExportJobRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CancelWorldExportJobOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CancelWorldExportJob(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::CancelWorldExportJobAsync(const CancelWorldExportJobRequest& request, const CancelWorldExportJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CancelWorldExportJobRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CancelWorldExportJob(*pRequest), context);
    } );
}

CancelWorldGenerationJobOutcome RoboMakerClient::CancelWorldGenerationJob(const CancelWorldGenerationJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CancelWorldGenerationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CancelWorldGenerationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/cancelWorldGenerationJob");
  return CancelWorldGenerationJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CancelWorldGenerationJobOutcomeCallable RoboMakerClient::CancelWorldGenerationJobCallable(const CancelWorldGenerationJobRequest& request) const
{
  std::shared_ptr<CancelWorldGenerationJobRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CancelWorldGenerationJobOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CancelWorldGenerationJob(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::CancelWorldGenerationJobAsync(const CancelWorldGenerationJobRequest& request, const CancelWorldGenerationJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CancelWorldGenerationJobRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CancelWorldGenerationJob(*pRequest), context);
    } );
}

CreateRobotApplicationOutcome RoboMakerClient::CreateRobotApplication(const CreateRobotApplicationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateRobotApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateRobotApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/createRobotApplication");
  return CreateRobotApplicationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateRobotApplicationOutcomeCallable RoboMakerClient::CreateRobotApplicationCallable(const CreateRobotApplicationRequest& request) const
{
  std::shared_ptr<CreateRobotApplicationRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateRobotApplicationOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateRobotApplication(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::CreateRobotApplicationAsync(const CreateRobotApplicationRequest& request, const CreateRobotApplicationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateRobotApplicationRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateRobotApplication(*pRequest), context);
    } );
}

CreateRobotApplicationVersionOutcome RoboMakerClient::CreateRobotApplicationVersion(const CreateRobotApplicationVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateRobotApplicationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateRobotApplicationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/createRobotApplicationVersion");
  return CreateRobotApplicationVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateRobotApplicationVersionOutcomeCallable RoboMakerClient::CreateRobotApplicationVersionCallable(const CreateRobotApplicationVersionRequest& request) const
{
  std::shared_ptr<CreateRobotApplicationVersionRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateRobotApplicationVersionOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateRobotApplicationVersion(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::CreateRobotApplicationVersionAsync(const CreateRobotApplicationVersionRequest& request, const CreateRobotApplicationVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateRobotApplicationVersionRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateRobotApplicationVersion(*pRequest), context);
    } );
}

CreateSimulationApplicationOutcome RoboMakerClient::CreateSimulationApplication(const CreateSimulationApplicationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSimulationApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSimulationApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/createSimulationApplication");
  return CreateSimulationApplicationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateSimulationApplicationOutcomeCallable RoboMakerClient::CreateSimulationApplicationCallable(const CreateSimulationApplicationRequest& request) const
{
  std::shared_ptr<CreateSimulationApplicationRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateSimulationApplicationOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateSimulationApplication(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::CreateSimulationApplicationAsync(const CreateSimulationApplicationRequest& request, const CreateSimulationApplicationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateSimulationApplicationRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateSimulationApplication(*pRequest), context);
    } );
}

CreateSimulationApplicationVersionOutcome RoboMakerClient::CreateSimulationApplicationVersion(const CreateSimulationApplicationVersionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSimulationApplicationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSimulationApplicationVersion, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/createSimulationApplicationVersion");
  return CreateSimulationApplicationVersionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateSimulationApplicationVersionOutcomeCallable RoboMakerClient::CreateSimulationApplicationVersionCallable(const CreateSimulationApplicationVersionRequest& request) const
{
  std::shared_ptr<CreateSimulationApplicationVersionRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateSimulationApplicationVersionOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateSimulationApplicationVersion(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::CreateSimulationApplicationVersionAsync(const CreateSimulationApplicationVersionRequest& request, const CreateSimulationApplicationVersionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateSimulationApplicationVersionRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateSimulationApplicationVersion(*pRequest), context);
    } );
}

CreateSimulationJobOutcome RoboMakerClient::CreateSimulationJob(const CreateSimulationJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSimulationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSimulationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/createSimulationJob");
  return CreateSimulationJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateSimulationJobOutcomeCallable RoboMakerClient::CreateSimulationJobCallable(const CreateSimulationJobRequest& request) const
{
  std::shared_ptr<CreateSimulationJobRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateSimulationJobOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateSimulationJob(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::CreateSimulationJobAsync(const CreateSimulationJobRequest& request, const CreateSimulationJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateSimulationJobRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateSimulationJob(*pRequest), context);
    } );
}

CreateWorldExportJobOutcome RoboMakerClient::CreateWorldExportJob(const CreateWorldExportJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateWorldExportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateWorldExportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/createWorldExportJob");
  return CreateWorldExportJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateWorldExportJobOutcomeCallable RoboMakerClient::CreateWorldExportJobCallable(const CreateWorldExportJobRequest& request) const
{
  std::shared_ptr<CreateWorldExportJobRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateWorldExportJobOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateWorldExportJob(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::CreateWorldExportJobAsync(const CreateWorldExportJobRequest& request, const CreateWorldExportJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateWorldExportJobRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateWorldExportJob(*pRequest), context);
    } );
}

CreateWorldGenerationJobOutcome RoboMakerClient::CreateWorldGenerationJob(const CreateWorldGenerationJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateWorldGenerationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateWorldGenerationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/createWorldGenerationJob");
  return CreateWorldGenerationJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateWorldGenerationJobOutcomeCallable RoboMakerClient::CreateWorldGenerationJobCallable(const CreateWorldGenerationJobRequest& request) const
{
  std::shared_ptr<CreateWorldGenerationJobRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateWorldGenerationJobOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateWorldGenerationJob(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::CreateWorldGenerationJobAsync(const CreateWorldGenerationJobRequest& request, const CreateWorldGenerationJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateWorldGenerationJobRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateWorldGenerationJob(*pRequest), context);
    } );
}

CreateWorldTemplateOutcome RoboMakerClient::CreateWorldTemplate(const CreateWorldTemplateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateWorldTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateWorldTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/createWorldTemplate");
  return CreateWorldTemplateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateWorldTemplateOutcomeCallable RoboMakerClient::CreateWorldTemplateCallable(const CreateWorldTemplateRequest& request) const
{
  std::shared_ptr<CreateWorldTemplateRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateWorldTemplateOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateWorldTemplate(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::CreateWorldTemplateAsync(const CreateWorldTemplateRequest& request, const CreateWorldTemplateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateWorldTemplateRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateWorldTemplate(*pRequest), context);
    } );
}

DeleteRobotApplicationOutcome RoboMakerClient::DeleteRobotApplication(const DeleteRobotApplicationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteRobotApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteRobotApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deleteRobotApplication");
  return DeleteRobotApplicationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteRobotApplicationOutcomeCallable RoboMakerClient::DeleteRobotApplicationCallable(const DeleteRobotApplicationRequest& request) const
{
  std::shared_ptr<DeleteRobotApplicationRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteRobotApplicationOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteRobotApplication(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::DeleteRobotApplicationAsync(const DeleteRobotApplicationRequest& request, const DeleteRobotApplicationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteRobotApplicationRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteRobotApplication(*pRequest), context);
    } );
}

DeleteSimulationApplicationOutcome RoboMakerClient::DeleteSimulationApplication(const DeleteSimulationApplicationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteSimulationApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteSimulationApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deleteSimulationApplication");
  return DeleteSimulationApplicationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteSimulationApplicationOutcomeCallable RoboMakerClient::DeleteSimulationApplicationCallable(const DeleteSimulationApplicationRequest& request) const
{
  std::shared_ptr<DeleteSimulationApplicationRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteSimulationApplicationOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteSimulationApplication(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::DeleteSimulationApplicationAsync(const DeleteSimulationApplicationRequest& request, const DeleteSimulationApplicationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteSimulationApplicationRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteSimulationApplication(*pRequest), context);
    } );
}

DeleteWorldTemplateOutcome RoboMakerClient::DeleteWorldTemplate(const DeleteWorldTemplateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteWorldTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteWorldTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/deleteWorldTemplate");
  return DeleteWorldTemplateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteWorldTemplateOutcomeCallable RoboMakerClient::DeleteWorldTemplateCallable(const DeleteWorldTemplateRequest& request) const
{
  std::shared_ptr<DeleteWorldTemplateRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteWorldTemplateOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteWorldTemplate(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::DeleteWorldTemplateAsync(const DeleteWorldTemplateRequest& request, const DeleteWorldTemplateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteWorldTemplateRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteWorldTemplate(*pRequest), context);
    } );
}

DescribeRobotApplicationOutcome RoboMakerClient::DescribeRobotApplication(const DescribeRobotApplicationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeRobotApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeRobotApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/describeRobotApplication");
  return DescribeRobotApplicationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeRobotApplicationOutcomeCallable RoboMakerClient::DescribeRobotApplicationCallable(const DescribeRobotApplicationRequest& request) const
{
  std::shared_ptr<DescribeRobotApplicationRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeRobotApplicationOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeRobotApplication(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::DescribeRobotApplicationAsync(const DescribeRobotApplicationRequest& request, const DescribeRobotApplicationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeRobotApplicationRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeRobotApplication(*pRequest), context);
    } );
}

DescribeSimulationApplicationOutcome RoboMakerClient::DescribeSimulationApplication(const DescribeSimulationApplicationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeSimulationApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeSimulationApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/describeSimulationApplication");
  return DescribeSimulationApplicationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeSimulationApplicationOutcomeCallable RoboMakerClient::DescribeSimulationApplicationCallable(const DescribeSimulationApplicationRequest& request) const
{
  std::shared_ptr<DescribeSimulationApplicationRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeSimulationApplicationOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeSimulationApplication(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::DescribeSimulationApplicationAsync(const DescribeSimulationApplicationRequest& request, const DescribeSimulationApplicationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeSimulationApplicationRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeSimulationApplication(*pRequest), context);
    } );
}

DescribeSimulationJobOutcome RoboMakerClient::DescribeSimulationJob(const DescribeSimulationJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeSimulationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeSimulationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/describeSimulationJob");
  return DescribeSimulationJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeSimulationJobOutcomeCallable RoboMakerClient::DescribeSimulationJobCallable(const DescribeSimulationJobRequest& request) const
{
  std::shared_ptr<DescribeSimulationJobRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeSimulationJobOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeSimulationJob(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::DescribeSimulationJobAsync(const DescribeSimulationJobRequest& request, const DescribeSimulationJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeSimulationJobRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeSimulationJob(*pRequest), context);
    } );
}

DescribeSimulationJobBatchOutcome RoboMakerClient::DescribeSimulationJobBatch(const DescribeSimulationJobBatchRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeSimulationJobBatch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeSimulationJobBatch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/describeSimulationJobBatch");
  return DescribeSimulationJobBatchOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeSimulationJobBatchOutcomeCallable RoboMakerClient::DescribeSimulationJobBatchCallable(const DescribeSimulationJobBatchRequest& request) const
{
  std::shared_ptr<DescribeSimulationJobBatchRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeSimulationJobBatchOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeSimulationJobBatch(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::DescribeSimulationJobBatchAsync(const DescribeSimulationJobBatchRequest& request, const DescribeSimulationJobBatchResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeSimulationJobBatchRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeSimulationJobBatch(*pRequest), context);
    } );
}

DescribeWorldOutcome RoboMakerClient::DescribeWorld(const DescribeWorldRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeWorld, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeWorld, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/describeWorld");
  return DescribeWorldOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeWorldOutcomeCallable RoboMakerClient::DescribeWorldCallable(const DescribeWorldRequest& request) const
{
  std::shared_ptr<DescribeWorldRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeWorldOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeWorld(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::DescribeWorldAsync(const DescribeWorldRequest& request, const DescribeWorldResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeWorldRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeWorld(*pRequest), context);
    } );
}

DescribeWorldExportJobOutcome RoboMakerClient::DescribeWorldExportJob(const DescribeWorldExportJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeWorldExportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeWorldExportJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/describeWorldExportJob");
  return DescribeWorldExportJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeWorldExportJobOutcomeCallable RoboMakerClient::DescribeWorldExportJobCallable(const DescribeWorldExportJobRequest& request) const
{
  std::shared_ptr<DescribeWorldExportJobRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeWorldExportJobOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeWorldExportJob(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::DescribeWorldExportJobAsync(const DescribeWorldExportJobRequest& request, const DescribeWorldExportJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeWorldExportJobRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeWorldExportJob(*pRequest), context);
    } );
}

DescribeWorldGenerationJobOutcome RoboMakerClient::DescribeWorldGenerationJob(const DescribeWorldGenerationJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeWorldGenerationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeWorldGenerationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/describeWorldGenerationJob");
  return DescribeWorldGenerationJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeWorldGenerationJobOutcomeCallable RoboMakerClient::DescribeWorldGenerationJobCallable(const DescribeWorldGenerationJobRequest& request) const
{
  std::shared_ptr<DescribeWorldGenerationJobRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeWorldGenerationJobOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeWorldGenerationJob(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::DescribeWorldGenerationJobAsync(const DescribeWorldGenerationJobRequest& request, const DescribeWorldGenerationJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeWorldGenerationJobRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeWorldGenerationJob(*pRequest), context);
    } );
}

DescribeWorldTemplateOutcome RoboMakerClient::DescribeWorldTemplate(const DescribeWorldTemplateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeWorldTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeWorldTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/describeWorldTemplate");
  return DescribeWorldTemplateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeWorldTemplateOutcomeCallable RoboMakerClient::DescribeWorldTemplateCallable(const DescribeWorldTemplateRequest& request) const
{
  std::shared_ptr<DescribeWorldTemplateRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeWorldTemplateOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeWorldTemplate(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::DescribeWorldTemplateAsync(const DescribeWorldTemplateRequest& request, const DescribeWorldTemplateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeWorldTemplateRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeWorldTemplate(*pRequest), context);
    } );
}

GetWorldTemplateBodyOutcome RoboMakerClient::GetWorldTemplateBody(const GetWorldTemplateBodyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetWorldTemplateBody, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetWorldTemplateBody, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/getWorldTemplateBody");
  return GetWorldTemplateBodyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetWorldTemplateBodyOutcomeCallable RoboMakerClient::GetWorldTemplateBodyCallable(const GetWorldTemplateBodyRequest& request) const
{
  std::shared_ptr<GetWorldTemplateBodyRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< GetWorldTemplateBodyOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->GetWorldTemplateBody(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::GetWorldTemplateBodyAsync(const GetWorldTemplateBodyRequest& request, const GetWorldTemplateBodyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<GetWorldTemplateBodyRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, GetWorldTemplateBody(*pRequest), context);
    } );
}

ListRobotApplicationsOutcome RoboMakerClient::ListRobotApplications(const ListRobotApplicationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListRobotApplications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListRobotApplications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/listRobotApplications");
  return ListRobotApplicationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListRobotApplicationsOutcomeCallable RoboMakerClient::ListRobotApplicationsCallable(const ListRobotApplicationsRequest& request) const
{
  std::shared_ptr<ListRobotApplicationsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListRobotApplicationsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListRobotApplications(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::ListRobotApplicationsAsync(const ListRobotApplicationsRequest& request, const ListRobotApplicationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListRobotApplicationsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListRobotApplications(*pRequest), context);
    } );
}

ListSimulationApplicationsOutcome RoboMakerClient::ListSimulationApplications(const ListSimulationApplicationsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListSimulationApplications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListSimulationApplications, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/listSimulationApplications");
  return ListSimulationApplicationsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListSimulationApplicationsOutcomeCallable RoboMakerClient::ListSimulationApplicationsCallable(const ListSimulationApplicationsRequest& request) const
{
  std::shared_ptr<ListSimulationApplicationsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListSimulationApplicationsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListSimulationApplications(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::ListSimulationApplicationsAsync(const ListSimulationApplicationsRequest& request, const ListSimulationApplicationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListSimulationApplicationsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListSimulationApplications(*pRequest), context);
    } );
}

ListSimulationJobBatchesOutcome RoboMakerClient::ListSimulationJobBatches(const ListSimulationJobBatchesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListSimulationJobBatches, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListSimulationJobBatches, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/listSimulationJobBatches");
  return ListSimulationJobBatchesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListSimulationJobBatchesOutcomeCallable RoboMakerClient::ListSimulationJobBatchesCallable(const ListSimulationJobBatchesRequest& request) const
{
  std::shared_ptr<ListSimulationJobBatchesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListSimulationJobBatchesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListSimulationJobBatches(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::ListSimulationJobBatchesAsync(const ListSimulationJobBatchesRequest& request, const ListSimulationJobBatchesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListSimulationJobBatchesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListSimulationJobBatches(*pRequest), context);
    } );
}

ListSimulationJobsOutcome RoboMakerClient::ListSimulationJobs(const ListSimulationJobsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListSimulationJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListSimulationJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/listSimulationJobs");
  return ListSimulationJobsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListSimulationJobsOutcomeCallable RoboMakerClient::ListSimulationJobsCallable(const ListSimulationJobsRequest& request) const
{
  std::shared_ptr<ListSimulationJobsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListSimulationJobsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListSimulationJobs(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::ListSimulationJobsAsync(const ListSimulationJobsRequest& request, const ListSimulationJobsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListSimulationJobsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListSimulationJobs(*pRequest), context);
    } );
}

ListTagsForResourceOutcome RoboMakerClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<RoboMakerErrors>(RoboMakerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable RoboMakerClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  std::shared_ptr<ListTagsForResourceRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListTagsForResourceOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListTagsForResource(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListTagsForResourceRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListTagsForResource(*pRequest), context);
    } );
}

ListWorldExportJobsOutcome RoboMakerClient::ListWorldExportJobs(const ListWorldExportJobsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListWorldExportJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListWorldExportJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/listWorldExportJobs");
  return ListWorldExportJobsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListWorldExportJobsOutcomeCallable RoboMakerClient::ListWorldExportJobsCallable(const ListWorldExportJobsRequest& request) const
{
  std::shared_ptr<ListWorldExportJobsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListWorldExportJobsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListWorldExportJobs(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::ListWorldExportJobsAsync(const ListWorldExportJobsRequest& request, const ListWorldExportJobsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListWorldExportJobsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListWorldExportJobs(*pRequest), context);
    } );
}

ListWorldGenerationJobsOutcome RoboMakerClient::ListWorldGenerationJobs(const ListWorldGenerationJobsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListWorldGenerationJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListWorldGenerationJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/listWorldGenerationJobs");
  return ListWorldGenerationJobsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListWorldGenerationJobsOutcomeCallable RoboMakerClient::ListWorldGenerationJobsCallable(const ListWorldGenerationJobsRequest& request) const
{
  std::shared_ptr<ListWorldGenerationJobsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListWorldGenerationJobsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListWorldGenerationJobs(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::ListWorldGenerationJobsAsync(const ListWorldGenerationJobsRequest& request, const ListWorldGenerationJobsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListWorldGenerationJobsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListWorldGenerationJobs(*pRequest), context);
    } );
}

ListWorldTemplatesOutcome RoboMakerClient::ListWorldTemplates(const ListWorldTemplatesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListWorldTemplates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListWorldTemplates, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/listWorldTemplates");
  return ListWorldTemplatesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListWorldTemplatesOutcomeCallable RoboMakerClient::ListWorldTemplatesCallable(const ListWorldTemplatesRequest& request) const
{
  std::shared_ptr<ListWorldTemplatesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListWorldTemplatesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListWorldTemplates(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::ListWorldTemplatesAsync(const ListWorldTemplatesRequest& request, const ListWorldTemplatesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListWorldTemplatesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListWorldTemplates(*pRequest), context);
    } );
}

ListWorldsOutcome RoboMakerClient::ListWorlds(const ListWorldsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListWorlds, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListWorlds, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/listWorlds");
  return ListWorldsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListWorldsOutcomeCallable RoboMakerClient::ListWorldsCallable(const ListWorldsRequest& request) const
{
  std::shared_ptr<ListWorldsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListWorldsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListWorlds(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::ListWorldsAsync(const ListWorldsRequest& request, const ListWorldsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListWorldsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListWorlds(*pRequest), context);
    } );
}

RestartSimulationJobOutcome RoboMakerClient::RestartSimulationJob(const RestartSimulationJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RestartSimulationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RestartSimulationJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/restartSimulationJob");
  return RestartSimulationJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RestartSimulationJobOutcomeCallable RoboMakerClient::RestartSimulationJobCallable(const RestartSimulationJobRequest& request) const
{
  std::shared_ptr<RestartSimulationJobRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< RestartSimulationJobOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->RestartSimulationJob(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::RestartSimulationJobAsync(const RestartSimulationJobRequest& request, const RestartSimulationJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<RestartSimulationJobRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, RestartSimulationJob(*pRequest), context);
    } );
}

StartSimulationJobBatchOutcome RoboMakerClient::StartSimulationJobBatch(const StartSimulationJobBatchRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartSimulationJobBatch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartSimulationJobBatch, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/startSimulationJobBatch");
  return StartSimulationJobBatchOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartSimulationJobBatchOutcomeCallable RoboMakerClient::StartSimulationJobBatchCallable(const StartSimulationJobBatchRequest& request) const
{
  std::shared_ptr<StartSimulationJobBatchRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< StartSimulationJobBatchOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->StartSimulationJobBatch(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::StartSimulationJobBatchAsync(const StartSimulationJobBatchRequest& request, const StartSimulationJobBatchResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<StartSimulationJobBatchRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, StartSimulationJobBatch(*pRequest), context);
    } );
}

TagResourceOutcome RoboMakerClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<RoboMakerErrors>(RoboMakerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable RoboMakerClient::TagResourceCallable(const TagResourceRequest& request) const
{
  std::shared_ptr<TagResourceRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< TagResourceOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->TagResource(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<TagResourceRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, TagResource(*pRequest), context);
    } );
}

UntagResourceOutcome RoboMakerClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<RoboMakerErrors>(RoboMakerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<RoboMakerErrors>(RoboMakerErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable RoboMakerClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  std::shared_ptr<UntagResourceRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UntagResourceOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UntagResource(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UntagResourceRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UntagResource(*pRequest), context);
    } );
}

UpdateRobotApplicationOutcome RoboMakerClient::UpdateRobotApplication(const UpdateRobotApplicationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateRobotApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateRobotApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/updateRobotApplication");
  return UpdateRobotApplicationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateRobotApplicationOutcomeCallable RoboMakerClient::UpdateRobotApplicationCallable(const UpdateRobotApplicationRequest& request) const
{
  std::shared_ptr<UpdateRobotApplicationRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateRobotApplicationOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateRobotApplication(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::UpdateRobotApplicationAsync(const UpdateRobotApplicationRequest& request, const UpdateRobotApplicationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateRobotApplicationRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateRobotApplication(*pRequest), context);
    } );
}

UpdateSimulationApplicationOutcome RoboMakerClient::UpdateSimulationApplication(const UpdateSimulationApplicationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateSimulationApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateSimulationApplication, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/updateSimulationApplication");
  return UpdateSimulationApplicationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateSimulationApplicationOutcomeCallable RoboMakerClient::UpdateSimulationApplicationCallable(const UpdateSimulationApplicationRequest& request) const
{
  std::shared_ptr<UpdateSimulationApplicationRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateSimulationApplicationOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateSimulationApplication(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::UpdateSimulationApplicationAsync(const UpdateSimulationApplicationRequest& request, const UpdateSimulationApplicationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateSimulationApplicationRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateSimulationApplication(*pRequest), context);
    } );
}

UpdateWorldTemplateOutcome RoboMakerClient::UpdateWorldTemplate(const UpdateWorldTemplateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateWorldTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateWorldTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/updateWorldTemplate");
  return UpdateWorldTemplateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateWorldTemplateOutcomeCallable RoboMakerClient::UpdateWorldTemplateCallable(const UpdateWorldTemplateRequest& request) const
{
  std::shared_ptr<UpdateWorldTemplateRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateWorldTemplateOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateWorldTemplate(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void RoboMakerClient::UpdateWorldTemplateAsync(const UpdateWorldTemplateRequest& request, const UpdateWorldTemplateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateWorldTemplateRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateWorldTemplate(*pRequest), context);
    } );
}

