using System;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json.Linq;
using UnityEngine;
using UnityEngine.Perception.GroundTruth;
using UnityEngine.Perception.GroundTruth.Consumers;
using UnityEngine.Perception.GroundTruth.DataModel;
using UnityEngine.Perception.Settings;

namespace MyCustomNamespace
{
    /// <summary>
    /// Example endpoint which outputs all information in small files to one single directory.
    /// We demonstrate two methods of serializing data: (1) Custom serialization; (2) IMessageProducers
    /// </summary>
    /// <remarks>For more complex examples, checkout SoloEndpoint</remarks>
    [Serializable]
    public class FlatEndpoint : IConsumerEndpoint, IFileSystemEndpoint
    {
        public string prefix = "FLAT";

        DateTimeOffset m_SimulationStartTime;
        private List<JToken> MetricData = new List<JToken>();
        /// Helper function to create a new JsonMessageBuilder
        JsonMessageBuilder GetNewJsonMessageBuilder() => new JsonMessageBuilder();
        
        private CommunicationServiceController _CommunicationServiceController;

        #region IFileSystemEndpoint

        /// <summary>
        /// Allows the user to set the base path from the Perception Settings UI.
        /// </summary>
        public string basePath
        {
            get => PerceptionSettings.GetOutputBasePath();
            set => PerceptionSettings.SetOutputBasePath(value);
        }

        string m_CachedCurrentPath = string.Empty;
        /// <summary>
        /// The root directory to use for all files that we output.
        /// </summary>
        public string currentPath
        {
            get
            {
                // Check if we already reserved the output path for this simulation run
                if (!string.IsNullOrWhiteSpace(m_CachedCurrentPath))
                    return m_CachedCurrentPath;

                // A small piece of logic to get the next available directory name
                //   get: {basePath}/FLAT_0
                //   if above already exists, then get: {basePath}/FLAT_1
                //   ... and so on

                var availableSuffix = 0;
                m_CachedCurrentPath = string.Empty;
                do
                {
                    m_CachedCurrentPath = Path.Combine(basePath, $"{prefix}_{availableSuffix}");
                    availableSuffix++;
                }
                while (Directory.Exists(m_CachedCurrentPath));

                // actually create the directory we decided upon above
                Directory.CreateDirectory(m_CachedCurrentPath);

                return m_CachedCurrentPath;
            }
        }

        /// <summary>
        /// The path used when "Reset to Default" is used in the Perception Settings UI.
        /// </summary>
        public string defaultPath => Path.Combine(Application.persistentDataPath);
        #endregion

        #region IConsumerEndpoint

        public string description => "Example endpoint that puts all the files in one single directory";

        /// <summary>
        /// Validate the configuration of your endpoint before the simulation runs.
        /// </summary>
        public bool IsValid(out string errorMessage)
        {
            // Check if the prefix supplied by the user is empty or whitespace
            if (string.IsNullOrWhiteSpace(prefix))
            {
                errorMessage = "Prefix must not be empty.";
                return false;
            }

            errorMessage = $"Directory {basePath} does not exist. Please create the directory.";
            // To create {basePath}/FLAT_{xyz}, we need to ensure that the {basePath} directory exists.
            // If it doesn't, the Perception Settings UI will show the above error message.
            return Directory.Exists(basePath);
        }

        public void SimulationStarted(SimulationMetadata metadata)
        {
            // record when the simulation started so we can use this to calculate
            // duration of the simulation on the SimulationEnded function
            m_SimulationStartTime = DateTimeOffset.Now;

        }

        public void SensorRegistered(SensorDefinition sensor)
        {
            //// Using Method 1 (Custom Serialization)
            // 1. Create a new JsonMessageBuilder class
            var builder = GetNewJsonMessageBuilder();

            // 2. Add all relevant fields to the builder as unique key-value pairs
            builder.AddString("model_type", sensor.modelType);
            builder.AddString("capture_mode", sensor.captureTriggerMode.ToString());
            builder.AddString("id", sensor.id);
            builder.AddInt("frames_between_captures", sensor.framesBetweenCaptures);
            builder.AddFloat("first_capture_frame", sensor.firstCaptureFrame);

            // Invariant: The builder now contains a representation of the sensor class
            // 3. We can use the ToJson function in the builder to write that representation to JSON
            // PathUtils.WriteAndReportJsonFile(
            //     Path.Combine(currentPath, $"sensor-{sensor.id}.json"),
            //     builder.ToJson()
            // );
        }

        public void AnnotationRegistered(AnnotationDefinition annotationDefinition)
        {
            
            //// Using Method 2 (IMessageProducer Serialization)

            // 1. Create a new JsonMessageBuilder class
            var builder = GetNewJsonMessageBuilder();
            

            // 2. Allow the annotation definition to convert itself to a message and
            // add it to our builder
            annotationDefinition.ToMessage(builder);

            JToken myJtoken = builder.ToJson();
            _CommunicationServiceController = UnityEngine.Object.FindObjectOfType<CommunicationServiceController>();
            _CommunicationServiceController.SetAnnotationData(myJtoken);

            foreach (var _label in myJtoken["spec"].Children()) 
            { 
                Debug.Log(_label.Value<string>("label_id"));
                Debug.Log(_label.Value<string>("label_name"));
            }
            // Invariant: The builder now contains a representation of the annotation definition class
            // 3. We can use the ToJson function in the builder to write that representation to JSON
            // PathUtils.WriteAndReportJsonFile(
            //     Path.Combine(currentPath, $"annotation-definition-{annotationDefinition.id}.json"),
            //     builder.ToJson()
            // );
        }

        public void MetricRegistered(MetricDefinition metricDefinition)
        {
            // Using Method 2 (IMessageProducer Serialization)

            // Similar to SensorDefinition, MetricDefinition also inherits from IMessageProducer
            // so it can tell the builder how it should be serialized.
            var builder = GetNewJsonMessageBuilder();
            metricDefinition.ToMessage(builder);
            JToken myJtoken = builder.ToJson();
            
            _CommunicationServiceController = UnityEngine.Object.FindObjectOfType<CommunicationServiceController>();
            _CommunicationServiceController.SetMetricData(myJtoken);
            MetricData.Add(myJtoken);

            
            // PathUtils.WriteAndReportJsonFile(
            //     Path.Combine(currentPath, $"annotation-definition-{metricDefinition.id}.json"),
            //     builder.ToJson()
            // );
        }

        public void FrameGenerated(Frame frame)
        {
            // Using Method 2 (IMessageProducer Serialization)

            // By default, the JsonMessageBuilder class does not know how to process image files referenced in the
            // Frame class. So we need to make a new FlatFrameMessageBuilder that inherits from JsonMessageBuilder
            // and specify how to handle image files. We can conveniently use the ToMessage function of the Frame
            // class and pass in our new FlatFrameMessageBuilder class.
            var builder = new FlatFrameMessageBuilder(this, frame);
            frame.ToMessage(builder);

            // PathUtils.WriteAndReportJsonFile(
            //     Path.Combine(currentPath, $"frame-{frame.id}.json"),
            //     builder.ToJson()
            // );
        }

        public void SimulationCompleted(SimulationMetadata metadata)
        {
            //// Using Method 2 (IMessageProducer Serialization)

            // 1. Create a new JsonMessageBuilder class
            var metadataBuilder = GetNewJsonMessageBuilder();

            // 2. Add metadata as a message into the metadataBuilder
            metadata.ToMessage(metadataBuilder);

            // 3. Write the metadata parameter to {currentPath}/metadata.json
            // PathUtils.WriteAndReportJsonFile(
            //     Path.Combine(currentPath, "metadata.json"),
            //     metadataBuilder.ToJson()
            // );

            //// Using Method 1 (Custom Serialization)

            // 1. Create a new JsonMessageBuilder class
            var completeBuilder = GetNewJsonMessageBuilder();
            var simulationEndTime = DateTimeOffset.Now;
            var simulationDuration = simulationEndTime - m_SimulationStartTime;

            // 2. Add all relevant key-value pairs
            completeBuilder.AddLong("start_timestamp", m_SimulationStartTime.ToUnixTimeMilliseconds());
            completeBuilder.AddLong("end_timestamp", m_SimulationStartTime.ToUnixTimeMilliseconds());
            completeBuilder.AddDouble("duration_seconds", simulationDuration.TotalSeconds);

            // JToken myJtoken = completeBuilder.ToJson();
            
            // _CommunicationServiceController = UnityEngine.Object.FindObjectOfType<CommunicationServiceController>();
            // _CommunicationServiceController.SetMetaData(myJtoken);
            // 3. Convert data to json and write to file
            // PathUtils.WriteAndReportJsonFile(
            //     Path.Combine(currentPath, "simulation-complete.json"),
            //     completeBuilder.ToJson()
            // );
        }

        /// <summary>
        /// Placeholder for crash resumption logic.
        /// </summary>
        /// <remarks>Unsupported for FlatEndpoint</remarks>
        public (string, int) ResumeSimulationFromCrash(int maxFrameCount)
        {
            Debug.LogError("Crash resumption not supported for FlatEndpoint output.");
            return (string.Empty, 0);
        }

        public object Clone()
        {
            return new FlatEndpoint();
        }

        #endregion
    }

    /// <summary>
    /// A MessageBuilder that extends JsonMessageBuilder to add support for serializing images and tensors.
    /// </summary>
    class FlatFrameMessageBuilder : JsonMessageBuilder
    {
        Frame m_Frame;
        FlatEndpoint m_Endpoint;

        public FlatFrameMessageBuilder(FlatEndpoint endpoint, Frame frame)
        {
            m_Endpoint = endpoint;
            m_Frame = frame;
        }

        /// <summary>
        /// Write out the byte array as an image and append sequence and step number to the key to construct
        /// the final file-name.
        /// </summary>
        public override void AddEncodedImage(string key, string extension, byte[] value)
        {
            if (value.Length > 0)
            {
                var filename = $"{key}_{m_Frame.sequence}-{m_Frame.step}.{extension.ToLower()}";
                // write out the file
                CommunicationServiceController _CommunicationServiceController = UnityEngine.Object.FindObjectOfType<CommunicationServiceController>();
                _CommunicationServiceController.SetFrameData(key,extension,value);
            //     PathUtils.WriteAndReportImageFile(
            //         Path.Combine(m_Endpoint.currentPath, filename),
            //         value
            //     );
            }
        }

        /// <summary>
        /// A nested message adds the output of an IMessageBuilder to a specific key.
        /// </summary>
        public override IMessageBuilder AddNestedMessage(string key)
        {
            var nested = new FlatFrameMessageBuilder(m_Endpoint, m_Frame);
            if (nestedValue.ContainsKey(key))
            {
                Debug.LogWarning($"Report data with key [{key}] will be overridden by new values");
            }

            nestedValue[key] = nested;
            return nested;
        }

        /// <summary>
        /// Adds the output of an IMessageBuilder as an element of an array
        /// identified by the key <see cref="arraykey"/>.
        /// </summary>
        public override IMessageBuilder AddNestedMessageToVector(string arraykey)
        {
            if (!nestedArrays.TryGetValue(arraykey, out var nestedList))
            {
                nestedList = new List<JsonMessageBuilder>();
                nestedArrays[arraykey] = nestedList;
            }
            var nested = new FlatFrameMessageBuilder(m_Endpoint, m_Frame);
            nestedList.Add(nested);
            return nested;
        }

        // A tensor is a multi-dimensional array
        public override void AddTensor(string key, Tensor tensor)
        {
            // By default, write the tensor as a flattened array
            currentJToken[key] = new JArray(tensor.buffer);
        }
    }
}