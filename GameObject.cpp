#include "GameObject.h"

GameObject::GameObject(std::string name)
{
    this->name = name;
    this->localPosition = Vector3D::Zero();
    this->localRotation = Vector3D::Zero();
    this->localScale = Vector3D::One();
    this->transform.setIdentity();
    this->tex = nullptr;
    this->parent = NULL;
    this->enabled = true;
    this->vertexShader = NULL;
    this->pixelShader = NULL;
}

GameObject::~GameObject()
{
}


std::string GameObject::GetName()
{
    return this->name;
}

void GameObject::SetPosition(float x, float y, float z)
{
    this->SetPosition(Vector3D(x, y, z));
}

void GameObject::SetPosition(Vector3D position)
{
    this->localPosition = position;
}

Vector3D GameObject::GetLocalPosition() const
{
    return this->localPosition;
}

void GameObject::SetScale(float x, float y, float z)
{
    this->SetScale(Vector3D(x, y, z));
}

void GameObject::SetScale(Vector3D scale)
{
    this->localScale = scale;
}

Vector3D GameObject::GetLocalScale() const
{
    return this->localScale;
}

void GameObject::SetRotation(float x, float y, float z)
{
    this->SetRotation(Vector3D(x, y, z));
}

void GameObject::SetRotation(Vector3D rotation)
{
    this->localRotation = rotation;
}

Vector3D GameObject::GetLocalRotation() const
{
    return this->localRotation;
}

void GameObject::SetTransform(const Matrix4x4& transform)
{
    this->transform.setMatrix(transform);
}

Vector3D GameObject::GetRight()
{
    return this->transform.GetRight();
}

Vector3D GameObject::GetUp()
{
    return this->transform.GetUp();
}

Vector3D GameObject::GetForward()
{
    return this->transform.GetForward();
}

Matrix4x4 GameObject::GetTransform()
{
    return this->transform;
}

void GameObject::MultiplyTransform(Matrix4x4 transform)
{
    this->transform *= transform;
}

GameObject* GameObject::GetParent()
{
    return this->parent;
}

GameObject* GameObject::GetChild(int index)
{
    return this->children[index];
}

std::vector<GameObject*> GameObject::GetChildren()
{
    return this->children;
}

void GameObject::SetTexture(const wchar_t* file_path)
{
    this->tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(file_path);
    if (!tex) throw std::exception("No texture created");
}

bool GameObject::IsEnabled() const
{
    return this->enabled;
}

void GameObject::setEnabled(bool enabled)
{
    this->enabled = enabled;
}


PixelShaderPtr GameObject::GetPixelShader()
{
    return this->pixelShader;
}

VertexShaderPtr GameObject::GetVertexShader()
{
    return this->vertexShader;
}

TexturePtr GameObject::GetTexture()
{
    return this->tex;
}

void GameObject::SetPixelShader(PixelShaderPtr pixelShader)
{
    this->pixelShader = pixelShader;
}

void GameObject::SetVertexShader(VertexShaderPtr vertexShader)
{
    this->vertexShader = vertexShader;
}
