/*
* Hdf5Utils.cpp
* Created on: 5/9/2024
* author : Moin Shaikh
*/

#include "Hdf5Utils.hpp"
using HighFive::File;
using HighFive::Group;

namespace MorphableModels
{
    namespace utils
    {

        
        File Hdf5Utils::openFile(const std::string& filename)
        {
            HighFive::File file(filename, HighFive::File::ReadWrite);
            try
            {
                file = HighFive::File(filename, HighFive::File::ReadOnly);
            }
            catch (const HighFive::Exception& e)
            {
                std::string msg = "Could not open HDF5 file \n" + std::string(e.what());
                throw msg;
            }
            return file;
        }

        Group Hdf5Utils::openPath(HighFive::File& file, const std::string& path)
        {
            HighFive::Group group;
            size_t curpos = 1;
            size_t nextpos = path.find_first_of("/", curpos);
            HighFive::Group G = file.getGroup("/");
            std::string name = path.substr(curpos, nextpos - 1);
            while (curpos != std::string::npos && !name.empty())
            {
                if (Hdf5Utils::existsObjectWithName(G, name))
                {
                    G = G.getGroup(name);
                }
                else
                {
                    std::string msg = "The path " + path + " does not exist";
                    throw msg.c_str();
                }

                curpos = nextpos + 1;
                nextpos = path.find_first_of("/", curpos);
                if (nextpos != std::string::npos)
                    name = path.substr(curpos, nextpos - curpos);
                else
                    name = path.substr(curpos);
            }
            return G;
        }

        cv::Mat Hdf5Utils::readMatrixFloat(const HighFive::Group& group, const std::string& name)
        {
            HighFive::DataSet ds = group.getDataSet(name);
            std::vector<size_t> dims = ds.getSpace().getDimensions();
            cv::Mat matrix(static_cast<int>(dims[0]), static_cast<int>(dims[1]), CV_32FC1);
            ds.read(matrix.data);
            return matrix;
        }

        void Hdf5Utils::readMatrixInt(const HighFive::Group& group, const std::string& name, cv::Mat& matrix)
        {
            HighFive::DataSet ds = group.getDataSet(name);
            std::vector<size_t> dims = ds.getSpace().getDimensions();
            matrix.create(static_cast<int>(dims[0]), static_cast<int>(dims[1]), CV_32SC1);
            ds.read(matrix.data);
            if (matrix.cols != 3)
                throw std::runtime_error("Reference reading failed, triangle-list has not 3 indices per entry");
        }

        void Hdf5Utils::readVector(const HighFive::Group& group, const std::string& name, std::vector<float>& vector)
        {
            HighFive::DataSet ds = group.getDataSet(name);
            std::vector<size_t> dims = ds.getSpace().getDimensions();
            vector.resize(dims[0]);
            ds.read(vector.data());
        }

        std::string Hdf5Utils::readString(const HighFive::Group& group, const std::string& name)
        {
            std::string outputString;
            HighFive::DataSet ds = group.getDataSet(name);
            ds.read(outputString);
            return outputString;
        }

        bool Hdf5Utils::existsObjectWithName(const HighFive::Group& group, const std::string& name)
        {
            for (const auto& objname : group.listObjectNames())
            {
                if (objname == name)
                {
                    return true;
                }
            }
            return false;
        }

        Render::Mesh Hdf5Utils::readFromHdf5(const std::string& filename)
        {
            Render::Mesh mesh;

            // Open the HDF5 file
            HighFive::File h5Model(filename, HighFive::File::ReadWrite);
            try
            {
                h5Model = HighFive::File(filename, HighFive::File::ReadOnly);
            }
            catch (const HighFive::Exception& e)
            {
                std::string msg = "Could not open HDF5 file \n" + std::string(e.what());
                throw msg.c_str();
            }

            // Load the mesh & color info
            HighFive::Group fg = h5Model.getGroup("/color");
            fg = fg.getGroup("representer/reference-mesh");

            // Vertex coordinates
            cv::Mat matVertex = Hdf5Utils::readMatrixFloat(fg, "./vertex-coordinates");
            if (matVertex.cols != 3)
                throw std::runtime_error("Reference reading failed, vertex-coordinates have too many dimensions");
            mesh.vertex.resize(matVertex.rows);
            for (int i = 0; i < matVertex.rows; ++i)
            {
                mesh.vertex[i].position = cv::Vec4f(matVertex.at<float>(i, 0), matVertex.at<float>(i, 1), matVertex.at<float>(i, 2), 1.0f);
            }

            // Triangle list
            cv::Mat matTL;
            Hdf5Utils::readMatrixInt(fg, "./triangle-list", matTL);
            if (matTL.cols != 3)
                throw std::runtime_error("Reference reading failed, triangle-list has not 3 indices per entry");
            mesh.tvi.resize(matTL.rows);
            for (int i = 0; i < matTL.rows; ++i)
            {
                mesh.tvi[i][0] = matTL.at<int>(i, 0);
                mesh.tvi[i][1] = matTL.at<int>(i, 1);
                mesh.tvi[i][2] = matTL.at<int>(i, 2);
            }

            // Color coordinates
            cv::Mat matColor = Hdf5Utils::readMatrixFloat(fg, "./vertex-colors");
            if (matColor.cols != 3)
                throw std::runtime_error("Reference reading failed, vertex-colors have too many dimensions");
            for (int i = 0; i < matColor.rows; ++i)
            {
                mesh.vertex[i].color = cv::Vec3f(matColor.at<float>(i, 2), matColor.at<float>(i, 1), matColor.at<float>(i, 0)); // order in hdf5: RGB. Order in OCV/vertex.color: BGR
            }

            mesh.hasTexture = false;

            return mesh;
        }
    }
}
