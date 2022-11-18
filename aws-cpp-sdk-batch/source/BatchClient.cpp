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

#include <aws/batch/BatchClient.h>
#include <aws/batch/BatchErrorMarshaller.h>
#include <aws/batch/BatchEndpointProvider.h>
#include <aws/batch/model/CancelJobRequest.h>
#include <aws/batch/model/CreateComputeEnvironmentRequest.h>
#include <aws/batch/model/CreateJobQueueRequest.h>
#include <aws/batch/model/CreateSchedulingPolicyRequest.h>
#include <aws/batch/model/DeleteComputeEnvironmentRequest.h>
#include <aws/batch/model/DeleteJobQueueRequest.h>
#include <aws/batch/model/DeleteSchedulingPolicyRequest.h>
#include <aws/batch/model/DeregisterJobDefinitionRequest.h>
#include <aws/batch/model/DescribeComputeEnvironmentsRequest.h>
#include <aws/batch/model/DescribeJobDefinitionsRequest.h>
#include <aws/batch/model/DescribeJobQueuesRequest.h>
#include <aws/batch/model/DescribeJobsRequest.h>
#include <aws/batch/model/DescribeSchedulingPoliciesRequest.h>
#include <aws/batch/model/ListJobsRequest.h>
#include <aws/batch/model/ListSchedulingPoliciesRequest.h>
#include <aws/batch/model/ListTagsForResourceRequest.h>
#include <aws/batch/model/RegisterJobDefinitionRequest.h>
#include <aws/batch/model/SubmitJobRequest.h>
#include <aws/batch/model/TagResourceRequest.h>
#include <aws/batch/model/TerminateJobRequest.h>
#include <aws/batch/model/UntagResourceRequest.h>
#include <aws/batch/model/UpdateComputeEnvironmentRequest.h>
#include <aws/batch/model/UpdateJobQueueRequest.h>
#include <aws/batch/model/UpdateSchedulingPolicyRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::Batch;
using namespace Aws::Batch::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* BatchClient::SERVICE_NAME = "batch";
const char* BatchClient::ALLOCATION_TAG = "BatchClient";

BatchClient::BatchClient(const Batch::BatchClientConfiguration& clientConfiguration,
                         std::shared_ptr<BatchEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<BatchErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

BatchClient::BatchClient(const AWSCredentials& credentials,
                         std::shared_ptr<BatchEndpointProviderBase> endpointProvider,
                         const Batch::BatchClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<BatchErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

BatchClient::BatchClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                         std::shared_ptr<BatchEndpointProviderBase> endpointProvider,
                         const Batch::BatchClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<BatchErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  BatchClient::BatchClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<BatchErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<BatchEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

BatchClient::BatchClient(const AWSCredentials& credentials,
                         const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<BatchErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<BatchEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

BatchClient::BatchClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                         const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<BatchErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<BatchEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
BatchClient::~BatchClient()
{
}

std::shared_ptr<BatchEndpointProviderBase>& BatchClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void BatchClient::init(const Batch::BatchClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Batch");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void BatchClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CancelJobOutcome BatchClient::CancelJob(const CancelJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CancelJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CancelJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/canceljob");
  return CancelJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CancelJobOutcomeCallable BatchClient::CancelJobCallable(const CancelJobRequest& request) const
{
  std::shared_ptr<CancelJobRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CancelJobOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CancelJob(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::CancelJobAsync(const CancelJobRequest& request, const CancelJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CancelJobRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CancelJob(*pRequest), context);
    } );
}

CreateComputeEnvironmentOutcome BatchClient::CreateComputeEnvironment(const CreateComputeEnvironmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateComputeEnvironment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateComputeEnvironment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/createcomputeenvironment");
  return CreateComputeEnvironmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateComputeEnvironmentOutcomeCallable BatchClient::CreateComputeEnvironmentCallable(const CreateComputeEnvironmentRequest& request) const
{
  std::shared_ptr<CreateComputeEnvironmentRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateComputeEnvironmentOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateComputeEnvironment(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::CreateComputeEnvironmentAsync(const CreateComputeEnvironmentRequest& request, const CreateComputeEnvironmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateComputeEnvironmentRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateComputeEnvironment(*pRequest), context);
    } );
}

CreateJobQueueOutcome BatchClient::CreateJobQueue(const CreateJobQueueRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateJobQueue, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateJobQueue, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/createjobqueue");
  return CreateJobQueueOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateJobQueueOutcomeCallable BatchClient::CreateJobQueueCallable(const CreateJobQueueRequest& request) const
{
  std::shared_ptr<CreateJobQueueRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateJobQueueOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateJobQueue(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::CreateJobQueueAsync(const CreateJobQueueRequest& request, const CreateJobQueueResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateJobQueueRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateJobQueue(*pRequest), context);
    } );
}

CreateSchedulingPolicyOutcome BatchClient::CreateSchedulingPolicy(const CreateSchedulingPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateSchedulingPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateSchedulingPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/createschedulingpolicy");
  return CreateSchedulingPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateSchedulingPolicyOutcomeCallable BatchClient::CreateSchedulingPolicyCallable(const CreateSchedulingPolicyRequest& request) const
{
  std::shared_ptr<CreateSchedulingPolicyRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< CreateSchedulingPolicyOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->CreateSchedulingPolicy(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::CreateSchedulingPolicyAsync(const CreateSchedulingPolicyRequest& request, const CreateSchedulingPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<CreateSchedulingPolicyRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, CreateSchedulingPolicy(*pRequest), context);
    } );
}

DeleteComputeEnvironmentOutcome BatchClient::DeleteComputeEnvironment(const DeleteComputeEnvironmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteComputeEnvironment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteComputeEnvironment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/deletecomputeenvironment");
  return DeleteComputeEnvironmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteComputeEnvironmentOutcomeCallable BatchClient::DeleteComputeEnvironmentCallable(const DeleteComputeEnvironmentRequest& request) const
{
  std::shared_ptr<DeleteComputeEnvironmentRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteComputeEnvironmentOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteComputeEnvironment(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::DeleteComputeEnvironmentAsync(const DeleteComputeEnvironmentRequest& request, const DeleteComputeEnvironmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteComputeEnvironmentRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteComputeEnvironment(*pRequest), context);
    } );
}

DeleteJobQueueOutcome BatchClient::DeleteJobQueue(const DeleteJobQueueRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteJobQueue, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteJobQueue, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/deletejobqueue");
  return DeleteJobQueueOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteJobQueueOutcomeCallable BatchClient::DeleteJobQueueCallable(const DeleteJobQueueRequest& request) const
{
  std::shared_ptr<DeleteJobQueueRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteJobQueueOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteJobQueue(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::DeleteJobQueueAsync(const DeleteJobQueueRequest& request, const DeleteJobQueueResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteJobQueueRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteJobQueue(*pRequest), context);
    } );
}

DeleteSchedulingPolicyOutcome BatchClient::DeleteSchedulingPolicy(const DeleteSchedulingPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteSchedulingPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteSchedulingPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/deleteschedulingpolicy");
  return DeleteSchedulingPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteSchedulingPolicyOutcomeCallable BatchClient::DeleteSchedulingPolicyCallable(const DeleteSchedulingPolicyRequest& request) const
{
  std::shared_ptr<DeleteSchedulingPolicyRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeleteSchedulingPolicyOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeleteSchedulingPolicy(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::DeleteSchedulingPolicyAsync(const DeleteSchedulingPolicyRequest& request, const DeleteSchedulingPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeleteSchedulingPolicyRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeleteSchedulingPolicy(*pRequest), context);
    } );
}

DeregisterJobDefinitionOutcome BatchClient::DeregisterJobDefinition(const DeregisterJobDefinitionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeregisterJobDefinition, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeregisterJobDefinition, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/deregisterjobdefinition");
  return DeregisterJobDefinitionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeregisterJobDefinitionOutcomeCallable BatchClient::DeregisterJobDefinitionCallable(const DeregisterJobDefinitionRequest& request) const
{
  std::shared_ptr<DeregisterJobDefinitionRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DeregisterJobDefinitionOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DeregisterJobDefinition(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::DeregisterJobDefinitionAsync(const DeregisterJobDefinitionRequest& request, const DeregisterJobDefinitionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DeregisterJobDefinitionRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DeregisterJobDefinition(*pRequest), context);
    } );
}

DescribeComputeEnvironmentsOutcome BatchClient::DescribeComputeEnvironments(const DescribeComputeEnvironmentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeComputeEnvironments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeComputeEnvironments, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/describecomputeenvironments");
  return DescribeComputeEnvironmentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeComputeEnvironmentsOutcomeCallable BatchClient::DescribeComputeEnvironmentsCallable(const DescribeComputeEnvironmentsRequest& request) const
{
  std::shared_ptr<DescribeComputeEnvironmentsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeComputeEnvironmentsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeComputeEnvironments(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::DescribeComputeEnvironmentsAsync(const DescribeComputeEnvironmentsRequest& request, const DescribeComputeEnvironmentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeComputeEnvironmentsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeComputeEnvironments(*pRequest), context);
    } );
}

DescribeJobDefinitionsOutcome BatchClient::DescribeJobDefinitions(const DescribeJobDefinitionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeJobDefinitions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeJobDefinitions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/describejobdefinitions");
  return DescribeJobDefinitionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeJobDefinitionsOutcomeCallable BatchClient::DescribeJobDefinitionsCallable(const DescribeJobDefinitionsRequest& request) const
{
  std::shared_ptr<DescribeJobDefinitionsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeJobDefinitionsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeJobDefinitions(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::DescribeJobDefinitionsAsync(const DescribeJobDefinitionsRequest& request, const DescribeJobDefinitionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeJobDefinitionsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeJobDefinitions(*pRequest), context);
    } );
}

DescribeJobQueuesOutcome BatchClient::DescribeJobQueues(const DescribeJobQueuesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeJobQueues, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeJobQueues, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/describejobqueues");
  return DescribeJobQueuesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeJobQueuesOutcomeCallable BatchClient::DescribeJobQueuesCallable(const DescribeJobQueuesRequest& request) const
{
  std::shared_ptr<DescribeJobQueuesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeJobQueuesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeJobQueues(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::DescribeJobQueuesAsync(const DescribeJobQueuesRequest& request, const DescribeJobQueuesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeJobQueuesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeJobQueues(*pRequest), context);
    } );
}

DescribeJobsOutcome BatchClient::DescribeJobs(const DescribeJobsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/describejobs");
  return DescribeJobsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeJobsOutcomeCallable BatchClient::DescribeJobsCallable(const DescribeJobsRequest& request) const
{
  std::shared_ptr<DescribeJobsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeJobsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeJobs(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::DescribeJobsAsync(const DescribeJobsRequest& request, const DescribeJobsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeJobsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeJobs(*pRequest), context);
    } );
}

DescribeSchedulingPoliciesOutcome BatchClient::DescribeSchedulingPolicies(const DescribeSchedulingPoliciesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeSchedulingPolicies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeSchedulingPolicies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/describeschedulingpolicies");
  return DescribeSchedulingPoliciesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeSchedulingPoliciesOutcomeCallable BatchClient::DescribeSchedulingPoliciesCallable(const DescribeSchedulingPoliciesRequest& request) const
{
  std::shared_ptr<DescribeSchedulingPoliciesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< DescribeSchedulingPoliciesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->DescribeSchedulingPolicies(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::DescribeSchedulingPoliciesAsync(const DescribeSchedulingPoliciesRequest& request, const DescribeSchedulingPoliciesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<DescribeSchedulingPoliciesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, DescribeSchedulingPolicies(*pRequest), context);
    } );
}

ListJobsOutcome BatchClient::ListJobs(const ListJobsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/listjobs");
  return ListJobsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListJobsOutcomeCallable BatchClient::ListJobsCallable(const ListJobsRequest& request) const
{
  std::shared_ptr<ListJobsRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListJobsOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListJobs(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::ListJobsAsync(const ListJobsRequest& request, const ListJobsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListJobsRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListJobs(*pRequest), context);
    } );
}

ListSchedulingPoliciesOutcome BatchClient::ListSchedulingPolicies(const ListSchedulingPoliciesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListSchedulingPolicies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListSchedulingPolicies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/listschedulingpolicies");
  return ListSchedulingPoliciesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListSchedulingPoliciesOutcomeCallable BatchClient::ListSchedulingPoliciesCallable(const ListSchedulingPoliciesRequest& request) const
{
  std::shared_ptr<ListSchedulingPoliciesRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListSchedulingPoliciesOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListSchedulingPolicies(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::ListSchedulingPoliciesAsync(const ListSchedulingPoliciesRequest& request, const ListSchedulingPoliciesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListSchedulingPoliciesRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListSchedulingPolicies(*pRequest), context);
    } );
}

ListTagsForResourceOutcome BatchClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<BatchErrors>(BatchErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable BatchClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  std::shared_ptr<ListTagsForResourceRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< ListTagsForResourceOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->ListTagsForResource(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<ListTagsForResourceRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, ListTagsForResource(*pRequest), context);
    } );
}

RegisterJobDefinitionOutcome BatchClient::RegisterJobDefinition(const RegisterJobDefinitionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RegisterJobDefinition, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RegisterJobDefinition, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/registerjobdefinition");
  return RegisterJobDefinitionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RegisterJobDefinitionOutcomeCallable BatchClient::RegisterJobDefinitionCallable(const RegisterJobDefinitionRequest& request) const
{
  std::shared_ptr<RegisterJobDefinitionRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< RegisterJobDefinitionOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->RegisterJobDefinition(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::RegisterJobDefinitionAsync(const RegisterJobDefinitionRequest& request, const RegisterJobDefinitionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<RegisterJobDefinitionRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, RegisterJobDefinition(*pRequest), context);
    } );
}

SubmitJobOutcome BatchClient::SubmitJob(const SubmitJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, SubmitJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, SubmitJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/submitjob");
  return SubmitJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

SubmitJobOutcomeCallable BatchClient::SubmitJobCallable(const SubmitJobRequest& request) const
{
  std::shared_ptr<SubmitJobRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< SubmitJobOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->SubmitJob(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::SubmitJobAsync(const SubmitJobRequest& request, const SubmitJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<SubmitJobRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, SubmitJob(*pRequest), context);
    } );
}

TagResourceOutcome BatchClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<BatchErrors>(BatchErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable BatchClient::TagResourceCallable(const TagResourceRequest& request) const
{
  std::shared_ptr<TagResourceRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< TagResourceOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->TagResource(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<TagResourceRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, TagResource(*pRequest), context);
    } );
}

TerminateJobOutcome BatchClient::TerminateJob(const TerminateJobRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TerminateJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TerminateJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/terminatejob");
  return TerminateJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TerminateJobOutcomeCallable BatchClient::TerminateJobCallable(const TerminateJobRequest& request) const
{
  std::shared_ptr<TerminateJobRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< TerminateJobOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->TerminateJob(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::TerminateJobAsync(const TerminateJobRequest& request, const TerminateJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<TerminateJobRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, TerminateJob(*pRequest), context);
    } );
}

UntagResourceOutcome BatchClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<BatchErrors>(BatchErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<BatchErrors>(BatchErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable BatchClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  std::shared_ptr<UntagResourceRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UntagResourceOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UntagResource(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UntagResourceRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UntagResource(*pRequest), context);
    } );
}

UpdateComputeEnvironmentOutcome BatchClient::UpdateComputeEnvironment(const UpdateComputeEnvironmentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateComputeEnvironment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateComputeEnvironment, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/updatecomputeenvironment");
  return UpdateComputeEnvironmentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateComputeEnvironmentOutcomeCallable BatchClient::UpdateComputeEnvironmentCallable(const UpdateComputeEnvironmentRequest& request) const
{
  std::shared_ptr<UpdateComputeEnvironmentRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateComputeEnvironmentOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateComputeEnvironment(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::UpdateComputeEnvironmentAsync(const UpdateComputeEnvironmentRequest& request, const UpdateComputeEnvironmentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateComputeEnvironmentRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateComputeEnvironment(*pRequest), context);
    } );
}

UpdateJobQueueOutcome BatchClient::UpdateJobQueue(const UpdateJobQueueRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateJobQueue, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateJobQueue, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/updatejobqueue");
  return UpdateJobQueueOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateJobQueueOutcomeCallable BatchClient::UpdateJobQueueCallable(const UpdateJobQueueRequest& request) const
{
  std::shared_ptr<UpdateJobQueueRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateJobQueueOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateJobQueue(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::UpdateJobQueueAsync(const UpdateJobQueueRequest& request, const UpdateJobQueueResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateJobQueueRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateJobQueue(*pRequest), context);
    } );
}

UpdateSchedulingPolicyOutcome BatchClient::UpdateSchedulingPolicy(const UpdateSchedulingPolicyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateSchedulingPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateSchedulingPolicy, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/updateschedulingpolicy");
  return UpdateSchedulingPolicyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateSchedulingPolicyOutcomeCallable BatchClient::UpdateSchedulingPolicyCallable(const UpdateSchedulingPolicyRequest& request) const
{
  std::shared_ptr<UpdateSchedulingPolicyRequest> pRequest = request.Clone();
  auto task = Aws::MakeShared< std::packaged_task< UpdateSchedulingPolicyOutcome() > >(ALLOCATION_TAG, [this, pRequest](){ return this->UpdateSchedulingPolicy(*pRequest); } );
  auto packagedFunction = [task]() { (*task)(); };
  m_executor->Submit(packagedFunction);
  return task->get_future();
}
void BatchClient::UpdateSchedulingPolicyAsync(const UpdateSchedulingPolicyRequest& request, const UpdateSchedulingPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  std::shared_ptr<UpdateSchedulingPolicyRequest> pRequest = request.Clone();
  m_executor->Submit( [this, pRequest, handler, context]()
    {
      handler(this, *pRequest, UpdateSchedulingPolicy(*pRequest), context);
    } );
}

