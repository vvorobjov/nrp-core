using System;
using System.IO;
using UnityEngine;
using UnityEngine.Perception.GroundTruth;

[RequireComponent(typeof(PerceptionCamera))]
public class ImageSender : MonoBehaviour
{

    public Camera cam;
    private Material mat;
    private RenderTexture renderTexture;
    public int bytesPerPixel = 3;
    private byte[] rawByteData;
    private Texture2D texture2D;
    private Rect rect;

    public byte[] GetCurrentFrame(int resWidth, int resHeight)
    {
        cam = GetComponent<Camera>();

        Byte[] bytes = new Byte[64];
        Array.Clear(bytes, 0, bytes.Length);

        renderTexture = new RenderTexture(resWidth, resHeight, 24);

        rawByteData = new byte[resWidth * resHeight * bytesPerPixel];
        texture2D = new Texture2D(resWidth, resHeight, TextureFormat.RGB24, false);
        rect = new Rect(0, 0, resWidth, resHeight);


        cam.targetTexture = renderTexture;
        cam.Render();
        RenderTexture.active = renderTexture;
        texture2D.ReadPixels(rect, 0, 0);
        bytes = texture2D.EncodeToPNG();

        Destroy(texture2D);
        Debug.Log(bytes);

        return bytes;
    }
}